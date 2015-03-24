// 打开一个xml文件读取其所有元素与属性
var f = new File(path.join(path.dirname(__FILE__), 'modelDescription.xml'));
f.open(File.ReadOnly);
var reader = new XmlStreamReader(f);
if (reader.readNext() == XmlStreamReader.StartDocument) {
    log('XML document version: ', reader.documentVersion());
    log('XML document encoding: ', reader.documentEncoding());
}
while (!reader.atEnd()) {
    if (reader.isStartElement()) {
        log('Element Name: ', reader.name());
        reader.attributes().forEach(function(x) {
            log('\t', x.name(), ' = ', x.value());
        });
    } 
    reader.readNext();
}
f.close();
