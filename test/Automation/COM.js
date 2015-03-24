rom.model = new rom.Assembly('model', null);
var model = rom.model;

var com = new rom.COM('COM组件', model);
var double1 = new rom.DoubleParameter('double1', rom.Input, com);
double1.value = 12.12;
var double2 = new rom.DoubleParameter('double2', rom.Input, com);
double2.value = 23.12;
var double3 = new rom.DoubleParameter('double3', rom.Output, com);

com.interactFunction = function() {
    self.loadCOMObject('{9F6F46E3-2F6B-4EA7-A68F-E1E697E2FC96}');
    //self.loadCOMObject('{16D0F7D6-F641-4362-A549-FC6436556794}');
    log('com is null: ', self.isNull());
    //获取com接口中的属性
    //log(self.comProperty('PI'));
    //调用com接口中的方法, 第一个参数是com接口方法的原型，第二个参数是一个数组，
    //将传递给com接口方法的参数.
    var res = self.callComFunction('AddNum(double,double)', 
            [self.double1.value, self.double2.value]); // 传入输入参数
    log(typeof res);
    log(res);
    self.double3.value = res; // 设置输出参数
/*
    //切换另一个com接口
    log('change the interface');
    res = self.loadCOMObject('MathUtils2Prog');
    //res = self.loadCOMObject('{68B8C912-ECCA-4015-8BA0-3B57D96F3BB5}');
    log(self.comProperty('magicNumber'));
    //设置com属性
    self.setComProperty('magicNumber', 12.1212);
    log(self.comProperty('magicNumber'));
    var res = self.callComFunction('multiply(double,doulbe)', [12, 12]);
    log(res);

    //输出com接口的文档
    //log(self.generateDocumentation());*/
};

