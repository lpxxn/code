var buf = new Buffer('仿立方');
log('buf:', buf);
var buf2 = new Buffer(buf);
log('buf2:', buf2);
buf.appendString('SimCube');
log('buf:', buf);
log('buf contains buf2?', buf.containsBuffer(buf2));
log('buf equals buf2?', buf.equals(buf2));
buf.chop(7);
log('buf equals buf2?', buf.equals(buf2));