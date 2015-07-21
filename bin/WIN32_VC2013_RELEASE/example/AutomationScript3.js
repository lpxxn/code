/* 使用AutomationScript仿函集成Isight中的aeroarg程序
 * 对待集成程序使用的输入，输出文件进行标记，用于自动提取输入，输出参数
 * 调用rom中的函数从模板文件中创建本地输入文件，并解析本地输出文件
 *
 * path对象连接文件路径，获取文件名称等。
 * 处理ChildProcess中发射的finished信号。
 */
rom.model = new rom.Assembly('model', null);
var model = rom.model;

var as = new rom.AutomationScript('aeroarg', model);
as.workDirectory = rom.SimFunction.InputDirectory | rom.SimFunction.OutputDirectory;

var app = new rom.FileParameter('app', rom.Input, as);
app.fileName = path.join(__dirname, 'aeroarg.exe');
app.description = '集成的应用程序';

var file1 = new rom.FileParameter('file1', rom.InputFile, as);
file1.fileName = path.join(__dirname, 'AeroIn.txt');
if (!fs.exists(file1.fileName))
    throw Error('输入文件不存在: ' + file1.fileName);
file1.isText = true;
file1.comment = '$';
file1.localFileName = path.filename(file1.fileName);
rom.createParametersFromFileParameter(file1);
var file2 = new rom.FileParameter('file2', rom.OutputFile, as);
file2.fileName = path.join(__dirname, 'AeroOut.txt');
if (!fs.exists(file2.fileName))
    throw Error('输出文件模板不存在: ' + file2.fileName);
file2.isText = true;
file2.comment = '$';
file2.localFileName = path.filename(file2.fileName);
rom.createParametersFromFileParameter(file2);

as.scriptFunction = function() { // 脚本仿函运行时执行的脚本
    // 集成的程序
    var appFile = self.app.fileName;
    if (!fs.exists(appFile)) 
        throw Error('集成的程序并不存在: ' + appFile);
    // 创建集成的程序的工作目录
    //self.createWorkDirectory(rom.InputDirectory | rom.OutputDirectory);
    // 根据输入参数创建输入文件
    var inputfilename = rom.createLocalInputTextFile(self.workDirectoryByFlag(rom.SimFunction.InputDirectory), self.file1);
    var outlocalfp = self.file2.localFileObject();
    outlocalfp.fileName = path.join(self.workDirectoryByFlag(rom.SimFunction.OutputDirectory), path.filename(outlocalfp.fileName));

    var cp = new ChildProcess();
    cp.finished.connect(function (exitCode, exitStatus) { // 在aeroarg程序执行完毕后调用此回调函数
        log('exitCode: ', exitCode, 'exitStatus', exitStatus);
        rom.parseLocalOutputTextFile(self.workDirectoryByFlag(rom.SimFunction.OutputDirectory), self.file2);
    });

    cp.spawn(appFile, [inputfilename, outlocalfp.fileName], {cwd: self.workDirectoryByFlag(rom.SimFunction.InputDirectory), waitForFinished: -1});
};

