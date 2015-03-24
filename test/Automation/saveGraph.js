/*测试由数据数组生成图表的功能*/

var x = [], y1 = [], y2 = [], y3 = [], yerror = [];

for (var i = 0; i < 6.28; i += 0.1) {
    x.push(i);
    y1.push(Math.sin(i));
    y2.push(Math.cos(i));
    y3.push(Math.sin(i) * Math.cos(i));
    yerror.push(Math.random() / 10);
}

log('使用一维数组生成单条正弦曲线的png文件');
ui.saveGraph(y1, path.join(__dirname, 'plot1.png'), {title:'单条正弦曲线'});

log('使用二维数组生成单条正弦曲线的jpg文件');
ui.saveGraph([x, y1], path.join(__dirname, 'plot2.jpg'), {title:'单条正弦曲线'});

log('使用二维数组生成单条余弦曲线的bmp文件');
ui.saveGraph([x, y1], path.join(__dirname, 'plot2.bmp'), {title:'单条正弦曲线'});

log('使用二维数组生成三条曲线的pdf文件');
ui.saveGraph([x, y1, y2, y3], path.join(__dirname, 'plot3.pdf'), {title:'三条曲线'});

log('使用二维数组生成三条曲线的svg文件');
ui.saveGraph([x, y1, y2, y3], path.join(__dirname, 'plot3.svg'), {title:'三条曲线'});

log('自定义图片尺寸，使用二维数组生成三条曲线的png文件');
ui.saveGraph([x, y1, y2, y3], path.join(__dirname, 'plot3.png'), {title:'棒图',
    size: '1024*768'});

log('自定义图片尺寸，使用二维数组生成棒图的png文件');
ui.saveGraph([x, y3], path.join(__dirname, 'plot4.png'), {title:'',
    size: '1024*768',
    type: ui.Stick});
    
log('使用二维数组生成带误差的曲线图');
ui.saveGraph([x, y1, yerror], path.join(__dirname, 'plot5'), {title:'误差图',
    significance: [ui.X, ui.Y, ui.YError]});
