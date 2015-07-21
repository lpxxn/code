rom.model = new rom.Assembly('model', null);
var model = rom.model;

var com = new rom.COM('COM组件', model);
var double1 = new rom.DoubleParameter('double1', rom.Input, com);
double1.value = 12.12;
var double2 = new rom.DoubleParameter('double2', rom.Input, com);
double2.value = 23.12;
var double3 = new rom.DoubleParameter('double3', rom.Output, com);
var file = new rom.FileParameter('comdoc', rom.Output, com);
file.isText = true;
file.fileName = path.join(__dirname, 'comdoc.html');

com.scriptFunction = function() {
    self.loadCOMObject('{3D1282A3-15AE-4F2A-B1D6-E1054834CBE3}');
    //self.loadCOMObject('MathCOM.SimCube');
    log('com is null: ', self.isNull());
    //调用com接口中的方法, 第一个参数是com接口方法的原型，第二个参数是一个数组，
    //将传递给com接口方法的参数.
    var res = self.callComFunction('AddNumber(double,double)', 
            [self.double1.value, self.double2.value]); // 传入输入参数
    log(typeof res);
    log(res);
    self.double3.value = res; // 设置输出参数

    //保存com接口的文档至html文件
    var f = new File(self.comdoc.fileName);
    if (!f.open(File.WriteOnly | File.Text))
        throw 'open file error.';
    f.writeString(self.generateDocumentation());
    f.close();
};

