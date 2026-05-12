import socket
import threading

# =========================================================
# CONFIGURACIÓN DEL SERVIDOR
# =========================================================

HOST = "0.0.0.0"   # Escucha en todas las interfaces
PORT = 9876        # Puerto TCP

# =========================================================
# MANEJO DE CLIENTES
# =========================================================

def handle_client(conn, addr):
    print(f"[CLIENT CONNECTED] {addr}")

    try:
        while True:

            # Recibe datos
            data = conn.recv(1024)

            # Cliente desconectado
            if not data:
                print(f"[CLIENT DISCONNECTED] {addr}")
                break

            # Decodifica ASCII
            message = data.decode('ascii', errors='ignore')

            print(f"[RX {addr}] -> {message}")

            # Respuesta opcional
            response = f"ACK: {message}"

            conn.send(response.encode('ascii'))

    except Exception as e:
        print(f"[ERROR] {addr} -> {e}")

    finally:
        conn.close()

# =========================================================
# SERVIDOR PRINCIPAL
# =========================================================

def start_server():

    # Crear socket TCP
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Reutilizar puerto rápidamente
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # Bind IP/PUERTO
    server.bind((HOST, PORT))

    # Máximo clientes en cola
    server.listen(5)

    print("===================================")
    print("      TCP SERVER STARTED")
    print("===================================")
    print(f"Listening on {HOST}:{PORT}")

    while True: 

        # Espera conexión cliente
        conn, addr = server.accept()

        # Hilo independiente por cliente
        client_thread = threading.Thread(
            target=handle_client,
            args=(conn, addr)
        )

        client_thread.daemon = True
        client_thread.start()

# =========================================================
# MAIN
# =========================================================

if __name__ == "__main__":
    start_server()