rom.model = new rom.Assembly('model', null);
var model = rom.model;
var loop1 = new rom.Loop('动态分析', model);
var script0 = new rom.Script('歼击机瞄准空中目标', loop1);

var input1 = new rom.DoubleParameter('Vv', rom.Input, script0);
input1.description = '目标飞行速度';
input1.units = 'km/h';
input1.value = 1000;
var input2 = new rom.DoubleParameter('Vu', rom.Input, script0);
input2.description = '歼击机飞行速度';
input2.units = 'km/h';
input2.value = 1500;
var input3 = new rom.DoubleParameter('Fv', rom.Input, script0);
input3.description = '歼击机飞行方向与目标飞行方向夹角';
input3.units = '度';
input3.value = 45;
var input4 = new rom.DoubleParameter('Fu', rom.Input, script0);
input4.description = '歼击机飞行方向与目标夹角';
input4.units = '度';
input4.value = 30;
var input5 = new rom.DoubleParameter('Do', rom.Input, script0);
input5.description = '歼击机初始发现目标距离';
input5.units = 'km';
input5.value = 10;
var input6 = new rom.DoubleParameter('Lp', rom.Input, script0);
input6.description = '导弹击中目标最远距离';
input6.units = 'km';
input6.value = 5;
var input7 = new rom.DoubleParameter('K', rom.Input, script0);
input7.description = '雷达性能参数';
input7.value = 3;

var input8 = new rom.DoubleParameter('Tck', rom.Input, script0);
input8.description = '雷达扫描时间间隔';
input8.units = 's';
input8.value = 90;

var output = new rom.DoubleParameter('Paim', rom.Output, script0);
output.description = '歼击机瞄准空中目标概率';
output.value = 0;

script0.scriptFunction = function() {
    Fv = Fv * Math.PI /180;
	Fu = Fu * Math.PI /180;
	var Vxd = Vu * Math.cos(Fu) - Vv * Math.cos(Fv);
	var n = 3.6 * 1000 *(Do - Lp)/(Vxd * Tck);
	var p = 1;
	for (var i=0; i<n; i++ )
	{
		p *= 1 - Math.exp(-K * Math.pow((Do-i*Vxd*Tck/3600)/Do,4));
	}
	Paim = 1 - p;

};

loop1.loopType = rom.Loop.For;
// 循环参数
loop1.setLoopParameterByGeneralParameter(input5);
loop1.setFrom(10);
loop1.setTo(100);
loop1.setIncrement(5);