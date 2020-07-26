#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>

gchar *buffer;

gboolean network_read(GIOChannel *source,
            GIOCondition cond,
            gpointer data)
{
    printf("inside network_read func. of server-async.c \n");
    GError *error=NULL;
    gsize len;
    GString *s = g_string_new(NULL);
    const gchar *msg = "The price of greatness is responsibility.";

    GIOStatus ret = g_io_channel_read_line_string(source, s, NULL, &error);
    if (ret == G_IO_STATUS_ERROR){
        g_error ("Error reading: %s\n", error->message);
        g_object_unref(data);
        return FALSE;
    }
    else if (ret == G_IO_STATUS_EOF) {
        g_print("finished\n");
        return FALSE;
    }
    else {
        g_print("Got: %s\n", s->str);

        //write
        ret = g_io_channel_write_chars(source, msg, -1, &len, &error);
        g_io_channel_flush(source, NULL);
        if (ret == G_IO_STATUS_ERROR)
                g_error ("Error writing: %s\n", error->message);
        printf ("Wrote %u bytes.\n", len);

        return TRUE;
    }
    printf("finished network_read func. of server-async.c \n");
}

gboolean
new_connection(GSocketService *service,
              GSocketConnection *connection,
              GObject *source_object,
              gpointer user_data)
{
    printf("inside new_connection func. of server-async.c \n");
    // tell glib not to disconnect
    g_object_ref(connection);
    GSocketAddress *sockaddr = g_socket_connection_get_remote_address(connection, NULL);
    GInetAddress *addr = g_inet_socket_address_get_address(G_INET_SOCKET_ADDRESS(sockaddr));
    guint16 port = g_inet_socket_address_get_port(G_INET_SOCKET_ADDRESS(sockaddr));

    g_print("New Connection from %s:%d\n", g_inet_address_to_string(addr), port);

    GSocket *socket = g_socket_connection_get_socket(connection);

    gint fd = g_socket_get_fd(socket);
    GIOChannel *channel = g_io_channel_unix_new(fd);
    g_io_add_watch(channel, G_IO_IN, (GIOFunc) network_read, connection);
    return TRUE;
    printf("finished new_connection func. of server-async.c \n");
}

int main(int argc, char **argv) 
{
    printf("inside main func. of server-async.c \n");
    GSocketService *service = g_socket_service_new();
    GInetAddress *address = g_inet_address_new_from_string("127.0.0.1");
    GSocketAddress *socket_address = g_inet_socket_address_new(address, 4000);
    g_socket_listener_add_address(G_SOCKET_LISTENER(service), socket_address, G_SOCKET_TYPE_STREAM,
            G_SOCKET_PROTOCOL_TCP, NULL, NULL, NULL);

    g_object_unref(socket_address);
    g_object_unref(address);
    g_socket_service_start(service);

    g_signal_connect(service, "incoming", G_CALLBACK(new_connection), NULL);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    
    //while(1) {
    
    //}
    printf("finished main func. of main.c \n");
}
