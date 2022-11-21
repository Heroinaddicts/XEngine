#include "MysqlPipe.h"

MysqlPipe::MysqlPipe(iMysqlSession* session, const std::string& host, const int port, const std::string& username, const std::string& password)
    : _Session(session), _Host(host), _Port(port), _Username(username), _Password(password), _Code(eErrorCode::UNKNOWN), _Handle(nullptr) {


}

void MysqlPipe::DoConnect() {

}
