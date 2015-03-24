rom.model = new rom.Assembly('model', null);
var model = rom.model;

var script0 = new rom.Script('计算程序', model);
var input1 = new rom.IntegerParameter('input1', rom.Input, script0);
input1.value = 12;
input1.enumEnabled = true;
input1.enumValues = [1, 2, 3];
//input1.enumNames = ['little', 'middle', 'big'];
input1.enumIndex = 2;

var input2 = new rom.DoubleParameter('input2', rom.Input, script0);
input2.value = 12;
input2.enumEnabled = true;
input2.enumValues = [11.1, 12.1, 13.1];
input2.enumNames = ['little', 'middle', 'big'];
input2.enumIndex = 1;

var output = new rom.DoubleParameter('output1', rom.Output, script0);

var strpar = new rom.StringParameter('str', rom.Input, script0);
strpar.value = 'hello';

var intarr = new rom.IntegerArrayParameter('intarr', rom.Input, [], script0);

var realarr = new rom.DoubleArrayParameter('realarr', rom.Input, [], script0);

script0.scriptFunction = function() {
    output1 = input1+ 100*Math.random();
};
