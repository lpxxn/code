// 测试与PostgreSQL数据库的操作

var db = sql.addDatabase('QPSQL', 'abc');
db.hostName = 'localhost';
db.databaseName = 'autotest';
db.userName = 'cubetan';
db.password = '123';
db.port = 5432;
if (db.open()) {
    log('open db is open');
    // 执行PostgreSQL数据库特有函数的查询
    var q = db.exec('select now(), timeofday()');
    if (q.first())
        log(q.value(0), q.value(1));
    db.close();
    sql.removeDatabase('abc');
} else {
    log('open db error');
}
