rom.model = new rom.Assembly('model', null);
var model = rom.model;

var as = new rom.AutomationScript('as', model);

var data1 = new rom.TableParameter('data1', rom.Input, as);
data1.appendColumn(rom.TableParameter.Integer, 3);
data1.appendColumn(rom.TableParameter.Float, 3);
data1.appendColumn(rom.TableParameter.Double, 3);
data1.appendColumn(rom.TableParameter.String, 3);
data1.appendColumn(rom.TableParameter.DateTime, 3);
data1.appendColumn(rom.TableParameter.FloatComplex, 3);
data1.appendColumn(rom.TableParameter.DoubleComplex, 3);

data1.fillColumnAt(0, [1,2,3]);
data1.setColumnNameAt(0, 'col1');
data1.setColumnLongNameAt(0, 'column 1');
data1.setColumnUnitAt(0, '秒');
data1.setColumnCommentAt(0, '整数列');
data1.setColumnSignificanceAt(0, rom.TableParameter.X);
data1.appendRowAt(0, 7928);
data1.insertRowAt(0, 1, 7128);

data1.fillColumnAt(1, [4.12,.125,-12.6]);
data1.setColumnNameAt(1, 'col2');
data1.setColumnLongNameAt(1, 'column 2');
data1.setColumnUnitAt(1, '秒');
data1.setColumnCommentAt(1, '浮点数列');
data1.setColumnSignificanceAt(1, rom.TableParameter.Y);
data1.appendRowAt(1, 79.28);
data1.insertRowAt(1, 1, 71.28);

data1.fillColumnAt(2, [4.12,.125,-12.6]);
data1.setColumnCommentAt(2, '双精度浮点数列');
data1.appendRowAt(2, 79.28);
data1.insertRowAt(2, 1, 71.28);

data1.fillColumnAt(3, ['abc', 'Hello World', 'Bye']);
data1.setColumnCommentAt(3, '字符串列');
data1.appendRowAt(3, "79.28");
data1.insertRowAt(3, 1, "71.28");

data1.fillColumnAt(4, [new Date(2014, 1, 2, 12, 23), 
        new Date(2011,11,3), new Date(2012,10,12)]);
data1.setColumnCommentAt(4, '日期时间列');
data1.appendRowAt(4, new Date(2014, 11,2));
data1.insertRowAt(4, 1, new Date(2014, 11, 3));

data1.fillColumnAt(5, [[1.1, 2.2], [3.3, 4.4], [5.5, 6.6]]);
data1.setColumnCommentAt(5, '浮点数复数列');
data1.appendRowAt(5, [79, 28]);
data1.insertRowAt(5, 1, [79, 18]);

data1.fillColumnAt(6, [[1.1, 2.2], [3.3, 4.4], [5.5, 6.6]]);
data1.setColumnCommentAt(6, '双精度浮点数复数列');
data1.appendRowAt(6, [79, 28]);
data1.insertRowAt(6, 4, [79, 18]);

var data2 = new rom.TableParameter('data2', rom.Output, as);

data1.swap(3, 5);
data1.swap(4, 6);

log(data1.integerColumnAt(0));
log(data1.floatColumnAt(1));
log(data1.doubleColumnAt(2));
log(data1.floatComplexColumnAt(3));
log(data1.doubleComplexColumnAt(4));
log(data1.stringColumnAt(5));
log(data1.dateTimeColumnAt(6));
