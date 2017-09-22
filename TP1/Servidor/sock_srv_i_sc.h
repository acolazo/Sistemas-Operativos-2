void sendfile_sc( int port_number, int * fd);
/**
 * @brief Levanta el servidor UDP y envia el archivo al cliente.
 *
 * @param port_number es el numero de puerto que se utiliza.
 * @param fd es un puntero a un arreglo de dimension 2 que se utiliza para sincronizar procesos con pipes.
 * 
 */
#define FILE_TO_SEND "datos.csv"
