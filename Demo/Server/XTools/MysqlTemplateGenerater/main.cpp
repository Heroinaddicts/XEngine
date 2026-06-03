#include "Header.h"
#include "CppGenerater.h"
#include "Mysql/mysql.h"
#include "Mysql/errmsg.h"

#ifdef WIN32
class AttachDumper {
public:
    AttachDumper() {
        XEngine::XDumper::GetInstance().SetDumpFilename((SafeSystem::Time::GetCurrentTimeString("%4d_%02d_%02d_%02d_%02d_%02d").append(".dmp")).c_str());
    }
};
static AttachDumper dumper;
#endif //WIN32

std::vector<TabInfo*> g_Tables;
std::map<std::string, std::string> g_CodeTemplates;

int main(int argc, const char** argv) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    Launcher::GetInstance()->ParseLauncherArgs(argc, argv);
    if (Launcher::GetInstance()->GetLauncherArg("Pause")) {
        getchar();
    }

    const std::string templates = GetLauncherParm(String, "Template");
    std::vector<std::string> paths, names;
    int count = SafeSystem::File::GetFileInDirectory(templates, ".template", paths, names);
    for (int i = 0; i < names.size(); i++) {
        std::string fileContent = SafeSystem::File::LoadFileAsString(paths[i]);
        g_CodeTemplates.insert(std::make_pair(names[i], fileContent));
    }

    const std::string host = GetLauncherParm(String, "Host");
    const int port = GetLauncherParm(Int32, "Port");
    const std::string db = GetLauncherParm(String, "Db");
    const std::string username = GetLauncherParm(String, "Username");
    const std::string password = GetLauncherParm(String, "Password");

    MYSQL* con = mysql_init(nullptr);
    XASSERT(con, "wtf");
    if (nullptr == con) {
        printf("mysql_init error\n");
        return -1;
    }

    my_bool enforce = 0;
    mysql_options(con, MYSQL_OPT_SSL_ENFORCE, &enforce);
    my_bool verify = 0;
    mysql_options(con, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &verify);
    if (mysql_real_connect(con, host.c_str(), username.c_str(), password.c_str(), db.c_str(), port, nullptr, CLIENT_MULTI_STATEMENTS) == nullptr) {
        printf("mysql_real_connect error %u: %s\n", mysql_errno(con), mysql_error(con));
        return -1;
    }

    bool reconnect = true;
    mysql_optionsv(con, MYSQL_OPT_RECONNECT, (void*)&reconnect);
    const char* sql = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = DATABASE()";
    if (mysql_query(con, sql) != 0) {
        printf("Failed to query tables: %s\n", mysql_error(con));
        return -1;
    }

    MYSQL_RES* result = mysql_store_result(con);
    if (!result) {
        printf("Failed to get tables result: %s\n", mysql_error(con));
        return -1;
    }

    std::vector<std::string> tableNames;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        if (row[0]) {
            printf("==================Table name %s=====================\n", row[0]);
            tableNames.push_back(row[0]);
        }
    }
    mysql_free_result(result);

    for (int i = 0; i < tableNames.size(); i++) {
        std::string sql = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = '" + tableNames[i] + "' AND CONSTRAINT_NAME = 'PRIMARY' ORDER BY ORDINAL_POSITION";
        if (mysql_query(con, sql.c_str()) != 0) {
            printf("Failed to query primary keys for %s, error %s", row[0], mysql_error(con));
            return -1;
        }


        MYSQL_RES* result = mysql_store_result(con);
        if (!result) {
            printf("Failed to get primary keys result: %s\n", mysql_error(con));
            return -1;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            if (row[0]) {
                printf("-----------Table %s Primary Key %s------------\n", tableNames[i].c_str(), row[0]);
            }
        }

        sql = "DESCRIBE " + tableNames[i];
        if (mysql_query(con, sql.c_str()) != 0) {
            printf("Failed to describe table %s : %s\n", tableNames[i], mysql_error(con));
            return -1;
        }

        result = mysql_store_result(con);
        if (!result) {
            printf("Failed to get table structure result:  %s\n", mysql_error(con));
            return -1;
        }

        MYSQL_FIELD* cols = mysql_fetch_fields(result);

        std::vector<ColumnInfo*> columnVector;
        ColumnInfo* primary = nullptr;
        int index = 0;
        while ((row = mysql_fetch_row(result))) {
            printf("%s|%s|%s|%s\n", row[0], row[1], row[2], row[3]);
            columnVector.push_back(txnew ColumnInfo(row[0], std::string(row[3]) == "PRI", std::string(row[3]) == "PRI" || std::string(row[3]) == "UNI", index, Type(row[1])));
            if (columnVector[index]->_Primary) {
                XASSERT(nullptr == primary, "wtf");
                primary = columnVector[index];
            }
            index++;
        }

        ColumnInfo** columns = txnew ColumnInfo * [index];
        for (int i = 0; i < index; i++) {
            columns[i] = columnVector[i];
        }

        TabInfo* tab = txnew TabInfo(tableNames[i], primary, columns, index);
        g_Tables.push_back(tab);
        mysql_free_result(result);
    }
    mysql_close(con);

    std::string cppCode = GeneratorCppCode();

    std::string savePath = GetLauncherParm(String, "Save");

    SafeSystem::File::SaveStringToFile(savePath, cppCode);
    return 0;
}
