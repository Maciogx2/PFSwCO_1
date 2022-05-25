#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
char response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<html><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js'></script><script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/2.2.4/jquery.min.js'></script><head> <title>IP & Time Zone</title> <style>.center{display: flex; justify-content: center; text-align: center; align-items: center; color: #fff; background-color: #000;}</style></head><body class='center'> <script>$.getJSON('http://ip-api.com/json', function (data){var dataParsed=JSON.parse(JSON.stringify(data, null, 2)); if(dataParsed['status']=='success'){$('#main').html('<h1>According to your IP address ' + dataParsed['query'] + '<br>your time zone is ' + dataParsed['timezone'] + '</h1>');}else{$('#main').html('<h1>Error: ' + dataParsed['message'] + '</h1>');}});</script> <div id='main'> Loading data... </div></body></html>";
int main()
{
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  char timeText[] = "                                      ";
  sprintf(timeText, "Start time: %s\n", asctime(timeinfo));
  syscall(SYS_write, STDOUT_FILENO, timeText, sizeof(timeText) - 1);

  char author[] = "Author: Maciej Rutkowski\r\n";
  syscall(SYS_write, STDOUT_FILENO, author, sizeof(author) - 1);
  int one = 1, client_fd;
  struct sockaddr_in svr_addr, cli_addr;
  socklen_t sin_len = sizeof(cli_addr);
 
  int sock = socket(AF_INET, SOCK_STREAM, 0); 
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
 
  int port = 3000;
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(port);
 
  if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
    close(sock);
  }
 
  listen(sock, 5);
  char portText[] = "                              ";
  sprintf(portText, "Now listening on: %d\n", port);
  syscall(SYS_write, STDOUT_FILENO, portText, sizeof(portText) - 1);
  while (1) {
    client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
 
    if (client_fd == -1) {
      continue;
    }
 
    write(client_fd, response, sizeof(response) - 1);
    close(client_fd);
  }
}