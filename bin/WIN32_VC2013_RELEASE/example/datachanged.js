// ????????????????????????????????x?y?v??????????
rom.model = new rom.Assembly('model');
var model = rom.model;

var loop = new rom.Loop('loop', model);
var sf = new rom.AutomationScript('sf', loop);
sf.workDirectory = rom.SimFunction.BaseDirectory;

var data1 = new rom.DoubleParameter('data1', rom.Input, sf);
var data2 = new rom.DoubleArrayParameter('data2', rom.Output, [4, 5], sf);

var data3 = new rom.TableParameter('data3', rom.Output, sf);
data3.appendColumn(rom.TableParameter.Double);
data3.appendColumn(rom.TableParameter.Double);
data3.appendColumn(rom.TableParameter.Double);
data3.setColumnSignificanceAt(0, rom.TableParameter.X);
data3.setColumnSignificanceAt(1, rom.TableParameter.Y);
data3.setColumnSignificanceAt(2, rom.TableParameter.V);

sf.scriptFunction = function() {
    var r, c, v1, v2, x, y, xcol = [],
        ycol = [],
        vcol = [];
    for (r = 0; r < 4; r++)
        for (c = 0; c < 5; c++)
            self.data2.setValue([r, c], Math.random());

    for (r = 0; r <= 10; r++)
        for (c = 0; c <= 10; c++) {
            x = .3 * (r - 5);
            y = .3 * (c - 5)
            xcol.push(x);
            ycol.push(y);
            v1 = x * x + (y - self.data1) * (y + self.data1);
            v2 = x * (y + self.data1) + x * (y + self.data1);
            vcol.push(1.0 / (v1 * v1 + v2 * v2));
        }
    self.data3.fillColumnAt(0, xcol);
    self.data3.fillColumnAt(1, ycol);
    self.data3.fillColumnAt(2, vcol);

//????????????????
    var filename = path.join(self.workDirectoryByFlag(rom.BaseDirectory), self.data1.toString() + '.txt');
	log(filename);
    var f = new File(filename);
    if (!f.open(File.WriteOnly | File.Text))
        throw 'open file error.';
    var ts = new TextStream(f);
    for (r = 0; r < xcol.length; r++)
        ts.writeString(xcol[r] + '\t' + ycol[r] + '\t' + vcol[r] + '\r\n');
    f.close();
    
    process.sleep(1);
};

loop.setLoopParameterByGeneralParameter(data1);
loop.setFrom(0.01);
loop.setIncrement(0.1);
loop.setTo(3.14);