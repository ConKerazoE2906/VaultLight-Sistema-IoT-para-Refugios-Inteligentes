import cv2
import numpy as np
import cv2.aruco as aruco
import requests
import time
import threading

# Datos de la API de Blynk
token = "EoA2IzHxFl1A8NaWFmf__etlQStsweG9"
pin_luz = "V6"

blynk_api_url_luz = f"https://blynk.cloud/external/api/get?token={token}&{pin_luz}"

# Variable global para almacenar el valor de luz
valor_luz = "N/A"

# Crear un objeto detector de marcadores
parametros = cv2.aruco.DetectorParameters()

# Crear un diccionario de marcadores ArUco
aruco_diccionario = aruco.getPredefinedDictionary(aruco.DICT_6X6_100)

# Inicializar la cámara
captura = cv2.VideoCapture(0)

# Cargar la imagen que quieres superponer en los marcadores
nueva_imagen = cv2.imread('luz.jpg')


def leerValores():
    """Función para obtener el valor de la API cada 10 segundos."""
    global valor_luz
    while True:
        try:
            respuesta = requests.get(blynk_api_url_luz)
            if respuesta.status_code == 200:
                valor_luz = respuesta.text  # Actualizar el valor de luz
            else:
                valor_luz = "Error API"
        except Exception:
            valor_luz = "Error Conexión"
        
        time.sleep(10)  # Esperar 10 segundos antes de la próxima solicitud


# Iniciar un hilo para obtener datos de la API sin bloquear el bucle principal
hilo_api = threading.Thread(target=leerValores, daemon=True)
hilo_api.start()


while True:
    # Capturar un fotograma de la cámara
    lectura, frame = captura.read()
    
    # Convertir el fotograma a escala de grises
    cuadro_gris = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Crear un objeto de detector ArUco
    detector = aruco.ArucoDetector(aruco_diccionario, parametros)

    # Detectar marcadores ArUco
    esquinas, identificador, puntosRechazados = detector.detectMarkers(cuadro_gris)

    if identificador is not None:
        # Dibujar los marcadores detectados
        aruco.drawDetectedMarkers(frame, esquinas, identificador)
        
        for i in range(len(identificador)):
            # Obtener las esquinas del marcador actual
            marker_corners = esquinas[i][0]
                
            # Definir la posición y el tamaño de la superposición
            x, y, w, h = cv2.boundingRect(marker_corners)

            # Ajustar el tamaño de la imagen para que coincida con el marcador
            nueva_imagen_resized = cv2.resize(nueva_imagen, (w, h))

            # Superponer la imagen en el área del marcador
            frame[y:y + h, x:x + w] = nueva_imagen_resized

            # Mostrar el valor de la API junto al marcador detectado
            texto = f"Luz: {valor_luz}"
            cv2.putText(frame, texto, (x, y - 10), cv2.FONT_HERSHEY_COMPLEX, 0.8, (0, 255, 0), 2)

    # Mostrar el resultado en una ventana
    cv2.imshow('Aruco', frame)
    
    # Salir con la tecla 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar la cámara y cerrar la ventana
captura.release()
cv2.destroyAllWindows()
