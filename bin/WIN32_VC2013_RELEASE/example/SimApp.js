rom.model = new rom.Assembly('model', null);
var model = rom.model;

var sa = new rom.SimApp('sa', model);

sa.appPath = path.join(path.dirname(__FILE__), 'aeroarg.exe');

var file1 = new rom.FileParameter('file1', rom.InputFile, sa);
file1.fileName = path.join(path.dirname(__FILE__), 'AeroIn.txt');
if (!fs.exists(file1.fileName))
    throw Error('输入文件不存在: ' + file1.fileName);
file1.isText = true;
file1.comment = '$';
rom.createParametersFromFileParameter(file1);
file1.localFileName = path.filename(file1.fileName);

var file2 = new rom.FileParameter('file2', rom.OutputFile, sa);
file2.fileName = path.join(path.dirname(__FILE__), 'AeroOut.txt');
if (!fs.exists(file2.fileName))
    throw Error('输出文件模板不存在: ' + file2.fileName);
file2.isText = true;
file2.comment = '$';
file2.localFileName = path.filename(file2.fileName);
rom.createParametersFromFileParameter(file2);

sa.args = [file1.fullName, file2.fullName];
