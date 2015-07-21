// 与com组件交互
var co = new ComObject();
//通过guid或progid设置com对象
//var res = co.setComObject('MathUtilsProg');
var res = co.setComObject('{16D0F7D6-F641-4362-A549-FC6436556794}');
log(res);
//获取com接口中的属性
log(co.comProperty('PI'));
//调用com接口中的方法, 第一个参数是com接口方法的原型，第二个参数是一个数组，
//将传递给com接口方法的参数.
//返回值res是一个数组, 第一个参数是com接口方法的返回值, 后面的是com接口方法
//的参数, 如果com接口方法有输出参数, 那么可以在这里获得输出值.
var res = co.callComFunction('AddNum(double,double)', [100, 102]);
log(typeof res);
//如果方法调用成功，那么返回一个数组
log(res);
log(res[0]);

//切换另一个com接口
res = co.setComObject('MathUtils2Prog');
//res = co.setComObject('{68B8C912-ECCA-4015-8BA0-3B57D96F3BB5}');
log(co.comProperty('magicNumber'));
//设置com属性
co.setComProperty('magicNumber', 12.1212);
log(co.comProperty('magicNumber'));
var res = co.callComFunction('multiply(double,doulbe)', [12, 12]);
log(res);

//输出com接口的文档
log(co.generateDocumentation());
