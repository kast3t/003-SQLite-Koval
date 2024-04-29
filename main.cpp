#include <iostream>
#include "sqlite3.h"

using namespace std;

const char* errMsg = NULL;

void deleteUrl(sqlite3* db, int64_t id) {
    sqlite3_stmt* pStmt;
    int prepareResult = sqlite3_prepare_v2(db, "DELETE FROM urls WHERE urls.id=?1", -1, &pStmt, NULL);
    if (prepareResult != SQLITE_OK) {
        errMsg = sqlite3_errmsg(db);
        throw "Ошибка подготовки запроса: ";
    }
    sqlite3_bind_int64(pStmt, 1, id);

    int stepResult = sqlite3_step(pStmt);
    if (stepResult != SQLITE_DONE) {
        throw "Ошибка выполнения запроса: ";
    }

    wcout << L"Строка с ID №" << id << L" успешно удалена" << endl;
    sqlite3_finalize(pStmt);
}

int main()
{
    setlocale(LC_ALL, "Russian");

    try {
        sqlite3* db;
        
        int openResult = sqlite3_open16(L"C:\\Users\\koval\\AppData\\Local\\Yandex\\YandexBrowser\\User Data\\Default\\History_test", &db); // Оригинальный файл - \\History
        if (openResult != SQLITE_OK) {
            errMsg = sqlite3_errmsg(db);
            throw "Ошибка открытия БД: ";
        }
        
        sqlite3_stmt* pStmt;
        int prepareResult = sqlite3_prepare_v2(db, "SELECT urls.id, urls.url, urls.title, urls.last_visit_time FROM urls", -1, &pStmt, NULL);
        if (prepareResult != SQLITE_OK) {
            errMsg = sqlite3_errmsg(db);
            throw "Ошибка подготовки запроса: ";
        }

        int stepResult = SQLITE_DONE;
        while (true) {
            int stepResult = sqlite3_step(pStmt);
            if (stepResult != SQLITE_ROW) {
                break;
            }

            int64_t id = sqlite3_column_int64(pStmt, 0);
            wchar_t* url = (wchar_t*)sqlite3_column_text16(pStmt, 1);
            wchar_t* title = (wchar_t*)sqlite3_column_text16(pStmt, 2);
            int64_t last_visit_time = sqlite3_column_int64(pStmt, 3);

            wcout << L"ID: " << id << endl;
            wcout << L"URL: " << url << endl;
            wcout << L"Название: " << title << endl;
            wcout << L"Последнее посещение: " << last_visit_time << endl << endl;
        }
        
        if (stepResult != SQLITE_DONE) {
            throw "Ошибка выполнения запроса: ";
        }
        sqlite3_finalize(pStmt);

        deleteUrl(db, 17);

        sqlite3_close(db);
    }
    catch (const char* errorMessage) {
		cout << errorMessage << errMsg << endl;
		return -1;
	}
}
