#include <string>
#include <limits>
#include <cmath>

#include <QImage>
#include <QVector3D>
#include <QColor>

#include "mosaic.hpp"
#include "mollusc.hpp"

QVector3D toVec3(const QColor& color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

const Mollusc& getClosestColor(const std::vector<Mollusc>& molluscs, const QVector3D& color)
{
    auto closestIndex = 0;
    auto minDist = std::numeric_limits<float>::max();

    for (auto i = 0u; i < molluscs.size(); ++i)
    {
        auto dist = (color - toVec3(molluscs[i].m_color)).length();
        if (dist < minDist)
        {
            minDist = dist;
            closestIndex = i;
        }
    }

    return molluscs[closestIndex];
}

QImage* createMosaic(const QImage& input, const std::vector<Mollusc>& molluscs)
{
    auto result = input.scaledToWidth(input.width() / 10, Qt::SmoothTransformation);

    auto width = result.width();
    auto height = result.height();

    std::vector<QVector3D> errorStorage((width + 2) * (height + 1), QVector3D());

    for (auto y = 0; y < result.height(); ++y)
    {
        for (auto x = 0; x < result.width(); ++x)
        {
            auto oldColor = QColor(result.pixel(x, y));
            auto oldVector = toVec3(oldColor) + errorStorage[x + 1 + y * width];

            const QColor& newColor = getClosestColor(molluscs, oldVector).m_color;
            auto newVector = toVec3(newColor);

            result.setPixel(x, y, newColor.rgb());
            auto error = oldVector - newVector;

            errorStorage[x + 2 + y * width] = error * 7 / 16;
            errorStorage[x + (y + 1) * width] = error * 3 / 16;
            errorStorage[x + 1 + (y + 1) * width] = error * 5 / 16;
            errorStorage[x + 2 + (y + 1) * width] = error * 1 / 16;
        }
    }

    return new QImage(result.scaledToWidth(width * 10));
}
