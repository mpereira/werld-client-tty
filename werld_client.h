#ifndef WERLD_CLIENT_WERLD_CLIENT_H
#define WERLD_CLIENT_WERLD_CLIENT_H

#define SERVER_ADDRESS "0.0.0.0"
#define SERVER_PORT 9876

void werld_client_connect(void);
void werld_client_disconnect(void);
int werld_client_send_player(struct player);

int fd;

#endif
