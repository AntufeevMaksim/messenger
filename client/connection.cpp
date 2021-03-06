#include "connection.h"

Connection::Connection(){
  Connect();
}

void Connection::Connect(){
  local.sin_family = AF_INET;
  local.sin_port = htons(8888);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  _sock = socket( AF_INET, SOCK_STREAM, 0);
  if (_sock < 0){
    perror("socket error");
  }
  int opt = 1;

  if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) == -1)
    perror("setsockopt");


  int rc = bind(_sock, (struct sockaddr*)&local, sizeof(local));
  if (rc<0){
    perror("error bind");
  }
  
  rc = listen(_sock, 5);
  if (rc){
    perror("listen error");
  }
  fcntl(_sock, F_SETFL, O_NONBLOCK);  
}

std::string Connection::Read(int s ){
  fd_set readfs;
  FD_ZERO(&readfs);
  FD_SET(s, &readfs);

  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 100;

  int n;

  int res = select(s+1, &readfs, NULL, NULL, &tv);
  if (res>0){
    ioctl(s, TIOCINQ, &n);
    std::vector<char> buf(n);
    int rc = recv(s, buf.data(), n, 0);
    if(rc>0){
      std::string answer;
      std::copy(buf.begin(), buf.end(),  std::back_inserter(answer));
      return answer;
    }
    return {};
  }
  else{
    return {};
  }
}

void Connection::Send(int s, std::vector<char>& buf){
  if (!buf.empty()){
    send(s, buf.data(), buf.size(), 0);
  }
}

void Connection::Send(int s, std::string& buf){
  if (!buf.empty()){
    buf += "#?#";
    try{
      send(s, buf.c_str(), buf.size(), 0);
    }
    catch(const std::exception& e){
      printf("exception");
    }
  }
}

int Connection::accept_new_connection(){
  int s = accept(_sock, NULL, NULL);
  return s;
}