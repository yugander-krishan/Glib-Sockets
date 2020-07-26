#include <glib.h>
#include <gio/gio.h>

static void
on_connected(GObject      *client,
             GAsyncResult *result,
             gpointer      user_data)
{
     printf("inside on_connected func. of client-async.c \n");
     gchar *buffer = user_data;
     GIOStream *conn;
     GError *error = NULL;
     conn = G_IO_STREAM(g_socket_client_connect_finish(G_SOCKET_CLIENT(client),
                                                      result, &error));
     
     if (conn == NULL) {
        printf("error in registering and it has failed \n");
     }else {
        printf("connection successful");
     }

     GInputStream * in_stream = g_io_stream_get_input_stream(G_IO_STREAM(conn));
     GOutputStream * out_stream = g_io_stream_get_output_stream(G_IO_STREAM(conn));
     g_output_stream_write(out_stream, user_data, 6, NULL, &error);

     gchar incoming_buff[100]={0};
     int incoming_num = g_input_stream_read(in_stream,incoming_buff,100,NULL,&error);

     if(error)
        g_error(error->message);
    else{
        g_print("incoming: %s\n", incoming_buff);
    }

}

int main(int argc, char *argv[])
{
    printf("inside main func. of client-async.c class \n");
    
    GError *error = NULL;

    GSocketClient * client = g_socket_client_new();
    
    GInetAddress * inetAddr = g_inet_address_new_from_string("127.0.0.1");

    GSocketAddress *address =  g_inet_socket_address_new(inetAddr, 4000);

    //GSocketConnection * connection = g_socket_client_connect_to_host (client,"127.0.0.1",4000,NULL,&error);
    
    /* synchronous connection works
    GSocketConnection * conn = g_socket_client_connect(client,
		                                       G_SOCKET_CONNECTABLE(address),
						       NULL,
						       &error);
    if(conn == NULL) {
       printf("conection is unsuccessful \n");
    }else {
       printf("connection is successful \n");
    }
    */

    
    g_socket_client_connect_async(client,
		                  G_SOCKET_CONNECTABLE(address),
                                  NULL,
                                  on_connected,
                                  "hello"				  
                                 );
     // infinite loop doesn't work for aync socket connect to server				 );
    //while(1) {
      //  printf("waiting for connection to server \n");
    //}
    
    // This is must for async socket connect to work
    //
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    //
    
    // Below also works 
    //GMainContext *context = g_main_context_default();
    //while(1) {
    //    g_main_context_iteration(context, TRUE);
    //}
    printf("end of main function of client-async.c class \n ");
     
    /*
    if (error){
        g_error(error->message);
    }else{
        g_message("Connection ok");
    }

    gchar *buffer="hello";
    gchar incoming_buff[100]={0};

    GInputStream * in_stream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    GOutputStream * out_stream = g_io_stream_get_output_stream(G_IO_STREAM(connection));
    g_output_stream_write(out_stream, buffer, 6, NULL, &error);
    int incoming_num = g_input_stream_read(in_stream,incoming_buff,100,NULL,&error);

    if(error)
        g_error(error->message);
    else{
        g_print("incoming: %s\n", incoming_buff);
    }
    */
    return TRUE;
}
