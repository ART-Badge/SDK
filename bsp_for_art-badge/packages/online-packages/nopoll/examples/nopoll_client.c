#include <rtthread.h>
#include <finsh.h>

#include <nopoll.h>
#include <nopoll_decl.h>
#include <nopoll_private.h>

#define HOST_NAME "echo.websocket.org"
#define HOST_PORT "80"

/* eg: "/ws" , used for http handshake request URI */
#define HOST_URI (NULL)

static char message[1024] = {'*'};
static char rev_buffer[1024];

static int nopoll_client(int argc, char **argv)
{
	noPollConn *conn = RT_NULL;
	noPollCtx *ctx = RT_NULL;

	/* initialize context */
	ctx = nopoll_ctx_new();

	/* create connection */
	conn = nopoll_conn_new(ctx, HOST_NAME, HOST_PORT, HOST_NAME, HOST_URI, NULL, NULL);
	if (!nopoll_conn_is_ok(conn))
	{
		printf("ERROR: Expected to find proper client connection status, but found error..\n");
		goto __exit;
	} /* end if */

	if (!nopoll_conn_wait_until_connection_ready(conn, 6))
	{
		printf("ERROR: websocket connection failed!\n");
		goto __exit;
	} /* end if */

	printf("web socket connection ready!\n");

	{
		int bytes_read;
		int retries = 0;
		int sendcnt = 0;

		int length = 0;

		/* test blocking mode */
		{
			char *block_msg = "Hello RT-Thread!";

			length = strlen(block_msg);

			/* send content text(utf-8) */
			printf("sending content..\n");
			if (nopoll_conn_send_text(conn, block_msg, length) != length)
			{
				printf("ERROR: Expected to find proper send operation..\n");
				goto __exit;
			}

			/* nopoll_true : blocking, wait for the reply (try to read length, blocking and with a 3 seconds timeout) */
			bytes_read = nopoll_conn_read(conn, rev_buffer, length, nopoll_true, 3000);
			if (bytes_read > 0)
				rev_buffer[bytes_read] = 0;

			if (bytes_read != length)
			{
				printf("ERROR: expected to find 14 bytes but found %d..\n", bytes_read);
				goto __exit;
			} /* end if */

			printf("recv: %s\n", rev_buffer);
		}

		memset(rev_buffer, 0, sizeof(rev_buffer));

		/* test no-blocking mode */
		{
			memset(message, '*', sizeof(message) - 1);

			char *send_message = message;

			while (1)
			{
				/* send content text(utf-8) */
				if (nopoll_conn_send_text(conn, send_message, strlen(send_message)) != strlen(send_message))
				{
					printf("ERROR: Expected to find proper send operation..\n");
					goto __exit;
				} /* end if */

				send_message += sizeof(char) * 10;
				sendcnt++;

				printf("sendcnt = %d\n", sendcnt);

				/* nopoll_false : non-blocking */
				bytes_read = nopoll_conn_read(conn, rev_buffer, sizeof(rev_buffer), nopoll_false, 1000);
				if (bytes_read < 0)
				{
					printf("ERROR: expected to find bytes from the connection but found: %d\n", bytes_read);
					goto __exit;
				}
				else if (bytes_read == 0)
				{
					retries++;
					if (retries > 10)
					{
						printf("Error: nothing found (0 bytes), %d retries\n", retries);
						goto __exit;
					} /* end if */

					continue;
				}
				else
				{
					rev_buffer[bytes_read] = '\0';
					printf("\nrecv: %s \nlength = %d\n", rev_buffer, strlen(rev_buffer));
				} /* end if */

				rt_thread_delay(3000);

				if ((send_message - message) / sizeof(char) > sizeof(message))
					goto __exit;
			}
		}
	}

__exit:
	/* close the connection */
	if (conn)
		nopoll_conn_close(conn);

	/* release context */
	if (ctx)
		nopoll_ctx_unref(ctx);

	return 0;
} /* end nopoll_client */
MSH_CMD_EXPORT(nopoll_client, websocket client test no input param);
