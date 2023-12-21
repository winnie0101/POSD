#include <gtest/gtest.h>
#include "../src/drawing.h"
#include "../src/painter.h"
#include "../src/shape.h"
#include "../src/triangle.h"

TEST(DrawingTest, testSetPainter){
    Painter * painter = new Painter("P001", "winnie");
    Painter * painter2 = new Painter("P002", "winnie2");
    std::list<Shape*> shapes;
    shapes.push_back(new Triangle(1, 1, 1));

    Drawing * drawing = new Drawing("D001", painter, shapes);
    drawing->setPainter(painter2);

    ASSERT_EQ("winnie2", drawing->painter()->name());

}

TEST(DrawingTest, testGetShape) {
    Painter * painter = new Painter("P001", "winnie");
    std::list<Shape*> shapes;
    shapes.push_back(new Triangle(1, 1, 1));

    Drawing * drawing = new Drawing("D001", painter, shapes);

    ASSERT_EQ("D001", drawing->id());
    ASSERT_EQ("winnie", drawing->painter()->name());
    ASSERT_EQ(3, drawing->getShape(0)->perimeter());
    ASSERT_EQ("triangle 1 1 1\n", drawing->getShapesAsString());

}