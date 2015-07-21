#include "package.h"
#include "./opc/packagereader.h"
#include "./opc/packagewriter.h"
#include "./parts/imagepart.h"
#include "./parts/headerorfooterpart.h"
#include "shared.h"

#include <QBuffer>
#include <QDebug>

namespace Docx {
Package::Package()
    : OpcPackage()
{
    PackURI packUri(QStringLiteral("/"));
    m_rels = new Relationships(packUri.baseURI());
    m_imageParts = new ImageParts();
    m_headerFooterParts = new HeaderAndFooterParts();
}

void Package::loadRel(const QString &reltype, const QString &targetRef, Part *target, const QString rId, bool isternal)
{
    m_rels->addRelationship(reltype, targetRef, target, rId, isternal);
}

Package *Package::open(const QString &pkgFile)
{
    PackageReader *reader = PackageReader::fromFile(pkgFile);
    //
    Package *package = new Package();
    Unmarshaller::unmarshal(reader, package);

    return package;
}

Package *Package::open(QIODevice *device)
{
    PackageReader *reader = PackageReader::fromFile(device);
    //
    Package *package = new Package();
    Unmarshaller::unmarshal(reader, package);

    return package;
}

void Package::save(const QString &filePath)
{
    PackageWriter p(filePath);
    QList<Part *> parts = this->parts();
    p.writeContentTypes(parts);
    p.writePkgRels(m_rels);
    p.writeParts(parts);
}

void Package::afterUnmarshal()
{
    QMap<QString, Relationship *> rels = m_rels->rels();

    gatherImageParts(rels);
    gatherHeaderAndFooterParts(rels);
}

ImageParts *Package::imageparts() const
{
    return m_imageParts;
}

HeaderAndFooterParts *Package::headerAndFooterParts() const
{
    return m_headerFooterParts;
}

Package::~Package()
{
    delete m_imageParts;
    delete m_headerFooterParts;
}

void Package::gatherImageParts(const QMap<QString, Relationship *> &rels)
{
    for (const Relationship *rel : rels.values()) {
        if (rel->relType() == Constants::IMAGE) {
            ImagePart *impart = static_cast<ImagePart *>(rel->target());

            m_imageParts->append(impart);
        }
        if (rel->target()) {
            Relationships *tarRels = rel->target()->rels();
            gatherImageParts(tarRels->rels());
        }
    }
}

void Package::gatherHeaderAndFooterParts(const QMap<QString, Relationship *> &rels)
{
    for (const Relationship *rel : rels.values()) {
        if (rel->relType() == Constants::HEADER_REL_TYPE) {
            HeaderPart *headPart = static_cast<HeaderPart *>(rel->target());

            m_headerFooterParts->appendHeader(headPart);
        }
        if (rel->relType() == Constants::FOOTER_REL_TYPE) {
            FooterPart *footerPart = static_cast<FooterPart *>(rel->target());

            m_headerFooterParts->appendFooter(footerPart);
        }
        if (rel->target()) {
            Relationships *tarRels = rel->target()->rels();
            gatherHeaderAndFooterParts(tarRels->rels());
        }
    }
}

ImageParts::ImageParts()
{

}

ImageParts::~ImageParts()
{
    qDeleteAll(m_imageparts);
    m_imageparts.clear();
}

/*!
 * \brief 得到或添加 ImagePart
 *
 *        查看是否有相应的 ImagePart 如果没有则新添加一下并返回
 * \param 图片路径
 * \return
 */
ImagePart *ImageParts::getOrAddImagePart(const PackURI &imageDescriptor)
{
    QByteArray key = getFileHash(imageDescriptor);
    ImagePart *part = getByHash(key);
    if (part)
        return part;

    Image *img = new Image(imageDescriptor);

    PackURI name = nextImagePartName(img->ext());
    part = ImagePart::fromImage(name, img, key);
    append(part);
    return part;
}

ImagePart *ImageParts::getOrAddImagePart(const QImage &img)
{
    QByteArray blob;
    QBuffer buffer(&blob);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG");
    Image *image = new Image(blob, QString::fromLatin1("PNG"));
    return getOrAddImagePart(image);
}

ImagePart *ImageParts::getOrAddImagePart(Image *img)
{
    QByteArray key = byteHash(img->blob());
    ImagePart *part = getByHash(key);
    if (part) {
        delete img;
        return part;
    }
    PackURI name = nextImagePartName(img->ext());
    part = ImagePart::fromImage(name, img, key);
    append(part);
    return part;
}

void ImageParts::append(ImagePart *item)
{
    if (m_imageparts.contains(item))
        return;
    m_imageparts.append(item);
}

/*!
 * \brief 是否有相同hash值的ImagePart
 * \param hash
 * \return
 */
ImagePart *ImageParts::getByHash(const QByteArray &hash)
{
    for (ImagePart *p : m_imageparts) {
        if (p->hash() == hash)
            return p;
    }
    return nullptr;
}

/*!
 * \brief 得到新图片的名称
 * \param ext 图片扩展名
 * \return
 */
PackURI ImageParts::nextImagePartName(const QString &ext)
{
    QList<int> numbers;
    for (ImagePart *p : m_imageparts) {
        numbers.append(p->partName().idx());
    }
    int num = 0;
    for (int i = 1, size = numbers.count() + 2; i < size; i++) {
        if (!numbers.contains(i)) {
            num = i;
            break;
        }
    }

    return PackURI(QString("word/media/image%1.%2").arg(num).arg(ext));
}

HeaderAndFooterParts::HeaderAndFooterParts()
{

}

void HeaderAndFooterParts::appendHeader(HeaderPart *part)
{
    m_headers.append(part);
}

void HeaderAndFooterParts::appendFooter(FooterPart *part)
{
    m_footers.append(part);
}

QList<HeaderPart *> HeaderAndFooterParts::headers() const
{
    return m_headers;
}

QList<FooterPart *> HeaderAndFooterParts::footers() const
{
    return m_footers;
}

HeaderAndFooterParts::~HeaderAndFooterParts()
{
    qDeleteAll(m_headers);
    m_headers.clear();
    qDeleteAll(m_footers);
    m_footers.clear();
}

}
