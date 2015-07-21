rom.model = new rom.Assembly('model', null);
var model = rom.model;

//进攻时搜索目标系效能
var scriptPfind = new rom.Script('进攻时搜索目标系效能', model);

var input1 = new rom.DoubleParameter('TExpect', rom.Input, scriptPfind);
input1.description = '平均期望时间';
input1.units = 's';
input1.value = 40;
var input2 = new rom.DoubleParameter('S', rom.Input, scriptPfind);
input2.description = '观察区域面积';
input2.units = 'km2';
input2.value = 40;
var input3 = new rom.DoubleParameter('B', rom.Input, scriptPfind);
input3.description = '观察带宽度';
input3.units = 'km';
input3.value = 3;
var input4 = new rom.DoubleParameter('V', rom.Input, scriptPfind);
input4.description = '侦察飞行速度';
input4.units = 'km/h';
input4.value = 1500;
var input5 = new rom.DoubleParameter('P0', rom.Input, scriptPfind);
input5.description = '侦察飞行速度';
input5.value = 0.95;
var input6 = new rom.DoubleParameter('L1', rom.Input, scriptPfind);
input6.description = '侦察机飞到侦察区域前路过敌区域的过程中防控火力的作用频度';
input6.value = 0.6;
var input7 = new rom.DoubleParameter('L2', rom.Input, scriptPfind);
input7.description = '侦察机在侦查区域完成任务后过敌区域的过程中防控火力的作用频度';
input7.value = 0.4;
var input8 = new rom.DoubleParameter('T1', rom.Input, scriptPfind);
input8.description = '侦察机飞到侦察区域前路过敌区域的平均飞行时间';
input8.units = 's';
input8.value = 600;
var input9 = new rom.DoubleParameter('T2', rom.Input, scriptPfind);
input9.description = '侦察机在侦查区域完成任务后过敌区域的平均飞行时间';
input9.units = 's';
input9.value = 600;
var input10 = new rom.DoubleParameter('Lwatch', rom.Input, scriptPfind);
input10.description = '侦查过程中防控火力的作用频度';
input10.value = 0.8;
var input11 = new rom.DoubleParameter('Twatch', rom.Input, scriptPfind);
input11.description = '在指定区域内完成观察的时刻';
input11.units = 's';
input11.value = 1000;

var output1 = new rom.DoubleParameter('Pfind', rom.Output, scriptPfind);
output1.description = '察机为歼击机提供可靠信息的概率';

scriptPfind.scriptFunction = function() {
	var Vmove = 1/TExpect;
	var Tg = S / (B * V) *3600;
	Pfind = 1/ (Vmove * Tg) * P0 * Math.exp(-(L1*T1+Lwatch*Twatch+L2*T2+Vmove*Tg)/3600) *(1-Math.exp(-Vmove * Twatch));

};




//精确侦察效能
var scriptU = new rom.Script('精确侦察效能', model);
var input12 = new rom.DoubleParameter('n', rom.Input, scriptU);
input12.description = '侦察飞行次数';
input12.value = 6;
var input13 = new rom.DoubleParameter('R', rom.Input, scriptU);
input13.description = '空地导弹杀伤区的圆半径';
input13.units = 'm';
input13.value = 5;
var input14 = new rom.DoubleParameter('Pfind', rom.Input, scriptU);
input14.description = '察机为歼击机提供可靠信息的概率';

var input15 = new rom.DoubleParameter('Deta', rom.Input, scriptU);
input15.description = '每次侦察飞行中确定目标的均方差';
input15.value = 10;

scriptU.createLinkByParameters(input14, output1);


var output2 = new rom.DoubleParameter('U', rom.Output, scriptU);
output2.description = '精确侦察效能';
var output3 = new rom.DoubleParameter('Wp', rom.Output, scriptU);
output3.description = '一次攻击目标的概率';

scriptU.scriptFunction = function() {
	var dtp = Deta / Math.sqrt(Pfind+1);
	var dtn = Deta / Math.sqrt(n*Pfind+1);
	Wp = 1 - Math.exp(-R*R*(Pfind+1)/(2*dtp*dtp));
	var Wn = 1 - Math.exp(-R*R*(n*Pfind+1)/(2*dtn*dtn));
	U = Wn /Wp;
};

//检验性侦察的效果
var scriptMn= new rom.Script('检验性侦察的效果', model);
var input16 = new rom.DoubleParameter('Wsure', rom.Input, scriptMn);
input16.description = '保证概率';
input16.value = 0.9;
var input17 = new rom.DoubleParameter('i', rom.Input, scriptMn);
input17.description = '攻击次数';
input17.value = 3;

var input18 = new rom.DoubleParameter('m', rom.Input, scriptMn);
input18.description = '一架飞机携带的弹的可能攻击次数';
input18.value = 3;

var input19 = new rom.DoubleParameter('Wp', rom.Input, scriptMn);
input19.description = '一次攻击目标的概率';

scriptMn.createLinkByParameters(input19, output3);

var output4 = new rom.DoubleParameter('Mn', rom.Output, scriptMn);
output4.description = '平均节余的火力数';


scriptMn.scriptFunction = function() {
	var num = Math.log(1-Wsure)/Math.log(1-Wp);
	Mn = (num-i)*m*(1-Math.pow((1-Wp),i));
	
};