/*
 * 数据库仿函示例
 * 使用sqlite数据库，在当前脚本所在路径创建一个文件名是abc.db3的数据库。
 * 数据库中只包含一个表，存储人名，年龄与体重信息。
 *
 */
rom.model = new rom.Assembly('model', null);
var model = rom.model;

// 若没有数据库，创建一个sqlite数据库，返回数据库中的姓名
var allnames = function() {
    var persons = [['张三', 21, 60], 
                   ['赵四', 22, 61], 
                   ['王五', 32, 81], 
                   ['马六', 12, 41]];
    var names = [];
    for (var i = 0; i < persons.length; ++i)
            names.push(persons[i][0]);

    var dbfile = path.join(__dirname, 'abc.db3');
    if (!fs.exists(dbfile)) {
        log('create db');
        var db = sql.addDatabase('QSQLITE', 'persons');
        db.databaseName = dbfile;
        if (!db.open()) {
            log('open db failed.');
            sql.removeDatabase('persons');
            return;
        }
        var table = 'CREATE TABLE persons (' +
                'id INTEGER PRIMARY KEY AUTOINCREMENT,' +
                'name VARCHAR(40) NOT NULL,' +
                'age INTEGER NOT NULL,' + 
                'weight INTEGER NOT NULL);';
        var q = db.exec(table);
        log(q.executedQuery);
        q.prepare('INSERT INTO persons (name, age, weight)' +
                'VALUES (:name, :age, :weight)');
        for (var i = 0, size = persons.length; i < size; ++i) {
            q.bindValue(':name', persons[i][0]);
            q.bindValue(':age', persons[i][1]);
            q.bindValue(':weight', persons[i][2]);
            q.exec();
        }
        log(q.executedQuery);
        db.close();
        sql.removeDatabase('persons');
    }
    return names;
}();

var dbSf = new rom.Database('数据库', model);
dbSf.driverName = 'QSQLITE'; // 设置数据库驱动器
dbSf.connectionName = dbSf.name; // 设置数据库连接名称
dbSf.databaseName = path.join(__dirname, 'abc.db3'); // 设置数据库名称, 对于sqlite数据库即文件名称

var name = new rom.StringParameter('Name', rom.Input, dbSf);
name.enumEnabled = true;
name.description = '姓名';
name.enumValues = allnames;
name.enumIndex = 0;

var age = new rom.IntegerParameter('Age', rom.Output, dbSf);
age.units = '岁';
age.description = '年龄';
var weight = new rom.IntegerParameter('Weight', rom.Output, dbSf);
weight.units = '公斤';
weight.description = '体重';

dbSf.interactFunction = function() { // 数据库仿函执行时执行的JavaScript代码
    var db = self.openedDatabase; // 如果上面的设置正确, 此时数据库已经打开
    var stat = 'SELECT age, weight FROM persons ' +
               'WHERE name = :name;';
    var q = db.exec(); // 从数据库对象中获得一个查询对象
    q.prepare(stat);
    q.bindValue(':name', self.Name.value);
    q.exec(); // 执行查询
    if (q.first()) {
        log(q.value(0), q.value('weight'));
        self.Age.value = q.value(0); // 从查询结果中设置输出参数的值
        self.Weight.value = q.value(1);
    }
};

