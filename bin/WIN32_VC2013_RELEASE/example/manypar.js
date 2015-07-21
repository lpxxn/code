// 创建模型
rom.model = new rom.Assembly('model');
var model = rom.model;
var sf = new rom.SimApp('sf', model);
sf.workDirectory = rom.BaseDirectory;
sf.appPath = 'notepad.exe';
var fpar = new rom.FileParameter('file', rom.InputFile, sf);
fpar.fileName = path.join(__dirname, 'manypar.txt');
fpar.isText = true;
fpar.isTemplate = true;
fpar.comment = '$';
fpar.localFileName = path.filename(fpar.fileName);
rom.createParametersFromFileParameter(fpar);
