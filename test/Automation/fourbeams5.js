rom.myCreateParameter = function(factory, mode, parent, name, value, desc, unit) {
	var tmpdata = new factory(name, mode, parent);
	tmpdata.value = value;
	tmpdata.description = desc;
	tmpdata.units = unit;
	return tmpdata;
};
 
rom.model = new rom.Assembly('model', null);
var model = rom.model;
var as = new rom.AutomationScript('������', model);
var data0 = new rom.IntegerParameter('Flag', rom.Input, as);
data0.enumEnabled = true;
data0.enumValues =��[1,2,3,4];
data0.enumNames  = ['������','��������','Բ����','��������'];
data0.enumIndex = 0;
var data1 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, as,'F',    100,   '�˵㼯����','N');
var data2 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, as,'E',    210000,'����ģ��','MPa');
var data3 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, as,'L',    100,   '���ĳ���','mm');
var data4 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, as,'H',    25,    '���ν���߶�','mm');
var data5 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, as,'B',    20,    '���ν�����','mm');
var data6 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,as,'disp',0,      '�˵�λ��','mm');
var data7 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,as,'angle',0,     '�Ӷ�','rad');
				
data0.respondChangedFunction = function() {
		var sf = self.owningSimfun;
		sf.name = '������';
    var deletPara = Object.getOwnPropertyNames(sf.parameters);
		for (var i = 4;i < deletPara.length; i++){		
				if(sf.parameters.hasOwnProperty(deletPara[i])){
						sf.remove(sf.parameters[deletPara[i]]);
				}	
		}	
		
    if (self.value == 1) {
        var sf = self.owningSimfun;  
        sf.name = '������';    	 
				var data4 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'H',    25,    '���ν���߶�','mm');
				var data5 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'B',    20,    '���ν�����','mm');
				var data6 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,sf,'disp',0,      '�˵�λ��','mm');
				var data7 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,sf,'angle',0,     '�Ӷ�','rad');    										  
    }
    if (self.value == 2) {
        var sf = self.owningSimfun;  
        sf.name = '��������';
				var data4 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'H',    25,    '�����ν���߶�','mm');
				var data5 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'B',    20,    '�����ν�����','mm');
				var data6 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,sf,'disp',0,      '�˵�λ��','mm');
				var data7 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,sf,'angle',0,     '�Ӷ�','rad');    													       													       
    }
    if (self.value == 3) {
        var sf = self.owningSimfun;
        sf.name = 'Բ����';   
				var data4 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'D',    20,    'Բ�ν�����','mm');
				var data5 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,sf,'disp',0,      '�˵�λ��','mm');
				var data6 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,sf,'angle',0,     '�Ӷ�','rad');    													       													       				     							
    }
    if (self.value == 4) {
        var sf = self.owningSimfun;
        sf.name = '��������'; 
				var data4 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'H',    25,    '�����ν���߶�','mm');
				var data5 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'B',    20,    '�����ν�����','mm');
				var data6 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'tw',    4,    '�����θ�����','mm');
				var data7 = rom.myCreateParameter(rom.DoubleParameter,rom.Input, sf,'t',    4,    '��������Ե���','mm');				
				var data8 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,sf,'disp',0,      '�˵�λ��','mm');
				var data9 = rom.myCreateParameter(rom.DoubleParameter,rom.Output,sf,'angle',0,     '�Ӷ�','rad');    													       													       				          					               							
    }       
}


as.scriptFunction = function() {
		if(self.Flag.value  == 1)
		{
				var Iy = self.B.value * Math.pow(self.H.value,3)/12;
				self.disp.value = self.F.value * Math.pow(self.L.value,3)/(3 * self.E.value * Iy);
				self.angle.value = self.F.value * Math.pow(self.L.value,2)/(2 * self.E.value * Iy);
		}
		if(self.Flag.value  == 2)
		{
				var Iy = self.B.value * Math.pow(self.H.value,3)/36;
				self.disp.value = self.F.value * Math.pow(self.L.value,3)/(3 * self.E.value * Iy); 
				self.angle.value = self.F.value * Math.pow(self.L.value,2)/(2 * self.E.value * Iy);
		}
		if(self.Flag.value  == 3)
		{
				var Iy = Math.PI * Math.pow(self.D.value,4)/64;
				self.disp.value = self.F.value * Math.pow(self.L.value,3)/(3 * self.E.value * Iy); 
				self.angle.value = self.F.value * Math.pow(self.L.value,2)/(2 * self.E.value * Iy);
		}
		if(self.Flag.value  == 4)
		{
				//var Iy = (B*H*H*H-(B-tw)*(H-2*t)*(H-2*t)*(H-2*t))/12;
				var tmp1 = Math.pow(self.H.value, 3);
				var tmp2 = Math.pow(self.H.value-2*self.t.value, 3);
				var Iy = (self.B.value * tmp1-(self.B.value - self.tw.value) * tmp2)/12;
				self.disp.value = self.F.value * Math.pow(self.L.value,3)/(3 * self.E.value * Iy); 
				self.angle.value = self.F.value * Math.pow(self.L.value,2)/(2 * self.E.value * Iy);
		}
};


