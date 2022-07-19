#ifndef DATABASE_H
#define DATABASE_H

#ifndef DIR_DB
#define DIR_DB "../materials"
#endif

FILE *connect(const char *db_name, const char *mode);
void disconnect(FILE *db);

#endif
