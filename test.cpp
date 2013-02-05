#include <stdio.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv){
    sqlite3 *db;
    sqlite3_backup *dbBackup;
    sqlite3 *pInMemory;

    char *zErrMsg = 0;
    int rc;

    if( argc != 2 ){
        fprintf(stderr, "Usage: %s DATABASE\n", argv[0]);
        return(1);
    }
    rc = sqlite3_open(argv[1], &db);
    if( SQLITE_OK == sqlite3_open(":memory:", &pInMemory) && rc == SQLITE_OK){
        dbBackup = sqlite3_backup_init(pInMemory, "main", db, "main");
        if (dbBackup){
            (void)sqlite3_backup_step(dbBackup, -1);
            sqlite3_exec(pInMemory, "UPDATE tbl1 SET one = 'hello!' WHERE two = 10", callback, 0, &zErrMsg);
            sqlite3_exec(pInMemory, "select * from tbl1", callback, 0, &zErrMsg);
            (void)sqlite3_backup_finish(dbBackup);
        }
    } else if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    } 

    printf("Here");
    rc = sqlite3_exec(db, "select * from tbl1", callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_close(pInMemory);
    return 0;
}
