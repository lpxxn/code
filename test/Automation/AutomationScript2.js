/* 使用AutomationScript仿函集成Isight中的aeroarg程序
 * 脚本仿函中的参数都是编写脚本直接创建，不是从输入，输出文件中提取
 * 在脚本执行时，获取输入参数的值，创建一个输入文件，调用aeroarg程序，产生一个输出文件
 * 解析输出文件，更新输出参数的值
 *
 * 使用了脚本中的File，TextStream类进行读、写文本文件，path对象连接文件路径，
 * 处理ChildProcess中发射的finished信号。
 */
rom.model = new rom.Assembly('model', null);
var model = rom.model;

var as = new rom.AutomationScript('as', model);

var app = new rom.FileParameter('app', rom.Input, as);
app.fileName = path.join(__dirname, 'aeroarg.exe');
app.description = '集成的应用程序';

var b = new rom.DoubleParameter('b', rom.Input, as);
b.value = 45;
b.units = 'ft';
b.description = 'Wing Span';

var swing = new rom.DoubleParameter('swing', rom.Input, as);
swing.value = 300;
swing.units = 'ft^2';
swing.description = 'Surface Area of the Wing';

var lfuse = new rom.DoubleParameter('lfuse', rom.Input, as);
lfuse.value = 20;
lfuse.units = 'ft';
lfuse.description = 'Length of Fuselage';

var dfuse = new rom.DoubleParameter('dfuse', rom.Input, as);
dfuse.value = 5;
dfuse.units = 'ft';
dfuse.description = 'Diameter of Fuselage';

var surfacearea = new rom.DoubleParameter('surfacearea', rom.Output, as);
surfacearea.units = 'ft^2';
surfacearea.description = 'Surface Area of the Fuselage';

var wettedarea = new rom.DoubleParameter('wettedarea', rom.Output, as);
wettedarea.units = 'ft^2';
wettedarea.description = 'Wetted Area of the Aircraft';

var liftdrag = new rom.DoubleParameter('liftdrag', rom.Output, as);
liftdrag.units = 'ft^2';
liftdrag.description = 'Lift/Drag of the Aircraft';

as.scriptFunction = function() { // 脚本仿函运行时执行的脚本
    // 集成的程序
    var appFile = self.app.fileName;
    if (!fs.exists(appFile)) 
        throw Error('集成的程序并不存在: ' + appFile);
        
    // 创建集成的程序的工作目录
    self.workDirectory = rom.InputDirectory | rom.OutputDirectory;
    // 根据输入参数创建输入文件
    var inputfilename = path.join(self.workDirectoryByFlag(rom.InputDirectory), 'AeroIn.txt');
    log('inpufile: ', inputfilename);
    var inputfile = new File(inputfilename);
    if (!inputfile.open(File.WriteOnly | File.Text))
        throw 'open file error.';
    var ts1 = new TextStream(inputfile);
    //ts1.setRealNumberPrecision(6);
    ts1.writeString('\n');
    ts1.writeString(' ==== Aerodynamic Analysis Input File ====');
    ts1.writeString('\n\n');
    ts1.writeString('Wing Span (ft)              b     =  ');
    ts1.writeNumber(self.b.value);
    ts1.writeString('\n');
    ts1.writeString('Surface Area of the Wing    Swing = ' + self.swing.value + '  ft^2');
    ts1.writeString('\n');
    ts1.writeString('Length of Fuselage          Lfuse =  ' + self.lfuse.value + '  ft');
    ts1.writeString('\n');
    ts1.writeString('Diameter of Fuselage        Dfuse =   ' + self.dfuse.value + '  ft');
    ts1.writeString('\n');
    inputfile.close();

    var cp = new ChildProcess();
    cp.finished.connect(function (exitCode, exitStatus) { // 在aeroarg程序执行完毕后调用此回调函数
        log('exitCode: ', exitCode, 'exitStatus', exitStatus);
        log('outputfile: ', outputfilename);
        var outfile = new File(outputfilename);
        if (!outfile.open(File.ReadOnly))
            throw 'open outputfile error.';
        var ts = new TextStream(outfile);
        log(ts.readLine());
        log(ts.readLine());
        log(ts.readLine());
        
        var reg = /([0-9.]+)/;
        var line = ts.readLine();
        log(line);
        self.surfacearea.value = Number(line.match(reg)[1]);

        line = ts.readLine();
        log(line);
        self.wettedarea.value = Number(line.match(reg)[1]);

        line = ts.readLine();
        log(line);
        self.liftdrag.value = Number(line.match(reg)[1]);
        outfile.close();
    });

    var outputfilename = path.join(self.workDirectoryByFlag(rom.OutputDirectory), 'AeroOut.txt');
    cp.spawn(appFile, [inputfilename, outputfilename], {cwd: self.workDirectoryByFlag(rom.InputDirectory), waitForFinished: -1});
};

