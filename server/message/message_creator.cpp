#include "message_creator.h"
#include "regular.h"
#include "get_all_users.h"
#include "broke_connection.h"
#include "get_users_status.h"
#include "set_user_name.h"
#include "initialize_user.h"
#include "get_user_name.h"

Message* MessageCreator::New(std::string message, int who_send_sock, Server *server){
  switch (message[0])
  {
  case 'r':
    return new Regular(message, who_send_sock, server);
    break;

  case 'g':
    return new GetAllUsers(message, who_send_sock, server);
    break;
    
  case 'b':
    return new BrokeConnection(message, who_send_sock, server);
    break;

  case 's':
    return new GetUsersStatus(message, who_send_sock, server);
    break;

  case 'n':
    return new SetUserName(message, who_send_sock, server);
    break;

  case 'i':
    return new InitializeUser(message, who_send_sock, server);
    break;

  case 'N':
    return new GetUserName(message, who_send_sock, server);
    break;

  default:
    std::__throw_invalid_argument(message.c_str());
    break;
  }
}

/*
r — regular message
g 
*/