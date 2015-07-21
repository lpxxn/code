rom.model = new rom.Assembly('model', null);
var model = rom.model;

var script = new rom.Script('script', model);

var com1 = new rom.CompositeParameter('com1', rom.Input, script);
var data1 = new rom.IntegerParameter('data1', rom.Input, com1);
var data2 = new rom.IntegerParameter('data2', rom.Input, com1);
var com2 = new rom.CompositeParameter('com2', rom.Output, script);
var data3 = new rom.IntegerParameter('data3', rom.Output, com2);
var data4 = new rom.IntegerParameter('data4', rom.Output, com2);
var file1 = new rom.FileParameter('file1', rom.InputFile, script);
var data5 = new rom.IntegerParameter('data5', rom.Input, file1);
var data6 = new rom.IntegerParameter('data6', rom.Input, file1);
var file2 = new rom.FileParameter('file2', rom.OutputFile, script);
var data7 = new rom.IntegerParameter('data7', rom.Output, file2);
var data8 = new rom.IntegerParameter('data8', rom.Output, file2);
