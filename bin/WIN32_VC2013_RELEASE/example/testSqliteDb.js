var db = sql.addDatabase('QSQLITE', 'abc');
db.databaseName = 'e:/abc.db3';
fs.unlink(db.databaseName);
if (db.open())
    log('open db is open');
var scooter = 'CREATE TABLE scooter (' +
              'id INTEGER PRIMARY KEY AUTOINCREMENT,' +
              'name VARCHAR(40) NOT NULL,' +
              'maxspeed INTEGER NOT NULL,' +
              'maxrange INTEGER NOT NULL,' +
              'weight INTEGER NOT NULL,' +
              'description VARCHAR(80) NOT NULL);';

var q = db.exec(scooter);
log(q.executedQuery);

q.prepare('INSERT INTO scooter (name, maxspeed, maxrange, weight, description)' +
          'VALUES (:name, :maxspeed, :maxrange, :weight, :description)');
q.bindValue(':name', 'scooter1');
q.bindValue(':maxspeed', 100);
q.bindValue(':maxrange', 120);
q.bindValue(':weight', 140);
q.bindValue(':description', '说明文字');
q.exec();
log(q.executedQuery);
log(q.lastQuery);
log(db);
db.close();
sql.removeDatabase('abc');
