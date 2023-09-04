#pragma once

#include <vector>
#include "utils.h"
#include "clipper2/clipper.h"
#include "ConcavePolygon.h"

namespace gamo
{
    namespace clip = Clipper2Lib;
    using AlgoPolygon = std::vector<Vect>;
    using PolygonList = std::vector<AlgoPolygon>;

    class PolygonAlgo
    {
    public:
        static PolygonList ComposePolygons(PolygonList polygons)
        {
            std::vector<std::vector<Vect>> united_polygons;
            clip::ClipperD clipper;

            clip::PathsD all_polygons_paths;
            for (int i = 0; i < polygons.size(); i++)
            {
                clip::PathD polygon_path;

                for (int j = 0; j < polygons[i].size(); j++)
                {
                    polygon_path.push_back(clip::PointD(polygons[i][j].x, polygons[i][j].y));
                }
                all_polygons_paths.push_back(polygon_path);
            }

            auto solutions = clip::Union(all_polygons_paths, clip::FillRule::NonZero);

            solutions = clip::InflatePaths(solutions, 0.0005, clip::JoinType::Miter, clip::EndType::Polygon);

            for (auto path : solutions)
            {
                std::vector<Vect> united_polygon;
                for (auto point : path)
                {
                    united_polygon.push_back(Vect{(float)point.x, (float)point.y});
                }
                united_polygons.push_back(united_polygon);
            }

            auto result = RemoveClosePointsInPolygons(united_polygons);
            return united_polygons;
        }

        static PolygonList DeComposePolygon(AlgoPolygon polygon)
        {
            std::vector<cxd::Vertex> input_vertices;
            for (auto point : polygon)
            {
                input_vertices.push_back(cxd::Vec2{point.x, point.y});
            }

            cxd::ConcavePolygon concavePoly(input_vertices);

            concavePoly.convexDecomp();

            std::vector<cxd::ConcavePolygon> polygons;
            concavePoly.returnLowestLevelPolys(polygons);

            std::vector<std::vector<Vect>> convexs;
            for (auto polygon : polygons)
            {
                std::vector<Vect> convex;
                for (auto v : polygon.getVertices())
                {
                    convex.push_back(Vect{(float)v.position.x, (float)v.position.y});
                }
                convexs.push_back(convex);
            }
            return convexs;
        }

    private:
        static PolygonList RemoveClosePointsInPolygons(PolygonList polygons)
        {
            float proximity_limit = 0.1f;

            std::vector<std::vector<Vect>> result_polygons;

            for (auto polygon : polygons)
            {
                std::vector<Vect> points_to_test = polygon;
                std::vector<Vect> points_to_remove;

                for (auto point_to_test : points_to_test)
                {
                    for (auto point : polygon)
                    {
                        if (point == point_to_test || std::find(points_to_remove.begin(), points_to_remove.end(), point) != points_to_remove.end())
                            continue;
                        bool close_in_x = fabs(point.x - point_to_test.x) < proximity_limit;
                        bool close_in_y = fabs(point.y - point_to_test.y) < proximity_limit;
                        if (close_in_x && close_in_y)
                        {
                            points_to_remove.push_back(point_to_test);
                            break;
                        }
                    }
                }
                // polygon.
                std::remove_if(polygon.begin(), polygon.end(), [&points_to_remove](Vect elm)
                               { return std::find(points_to_remove.begin(), points_to_remove.end(), elm) != points_to_remove.end(); });

                if (polygon.size() >= 3)
                {
                    result_polygons.push_back(polygon);
                }
            }
            return result_polygons;
        }
    };

} // namespace gamo
