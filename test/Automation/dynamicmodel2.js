/*
 *��̬����ģ��
 *��ʼģ����һ���ű��º���ֻ��һ�����˫���ȸ�����������ͨ�������������һ��0-100��ֵ��
 *�ű��º���һ���������������жϽű�ִ�к������Ƿ�С��95���������̬������ǰ�Ľű��º���
 *����������ģ�͵ĺ��棬ģ��ִ�����潫����ִ�ж�̬�����Ľű��º���������ֵ��С��95����ô���������µĽű��º���
 *������̽����������ֵ��С��95.
 */

rom.model = new rom.Assembly('model', null);
var sf = new rom.AutomationScript('����', rom.model);
var data = new rom.DoubleParameter('data', rom.Output, sf);
sf.scriptFunction = function() {
    self.data.value = Math.random() * 100;
}

sf.postProcessingFunction = function() {
    if (self.data.value < 95) {
        self.dynamicModifyModel(function() {
            var clonedSf = rom.$.cloneSimfunByJSON('{' + self.toJSON() + '}');
            clonedSf.name = '����' + rom.model.simfunCount();
            rom.model.insert(rom.model.simfunCount() - 1, clonedSf);
        });
    }
}				
