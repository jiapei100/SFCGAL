#include <boost/test/unit_test.hpp>

#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <SFCGAL/all.h>
#include <SFCGAL/io/wkt.h>
#include <SFCGAL/io/WktWriter.h>
#include <SFCGAL/algorithm/intersects.h>
#include <SFCGAL/transform/AffineTransform3.h>

using namespace SFCGAL ;

// always after CGAL
using namespace boost::unit_test ;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

BOOST_AUTO_TEST_SUITE( SFCGAL_algorithm_IntersectsTest )

BOOST_AUTO_TEST_CASE( testPointPointIntersects )
{
    Point pta( 0.0, 1.0, 0.0 );
    Point ptb( 0.0, 1.0, 0.0 );
    Point ptc( 0.0, 0.0, 0.0 );
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, ptb ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, ptc ), false );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( pta, ptb ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( pta, ptc ), false );
}

BOOST_AUTO_TEST_CASE( testPointLineStringIntersects )
{
    Point pta( 0.0, 0.0, 0.0 );
    Point ptb( 0.5, 0.0, 0.0 );
    Point ptc( 0.0, 0.0, 1.0 );
    std::auto_ptr<Geometry> ls = io::readWkt( "LINESTRING(0 0 0, 1 0 0, 1 1 0)" );
    // point on an extremity
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, *ls ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( pta, *ls ), true );
    // point on a line
    BOOST_CHECK_EQUAL( algorithm::intersects( ptb, *ls ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( ptb, *ls ), true );
    // point not on a line
    BOOST_CHECK_EQUAL( algorithm::intersects( ptc, *ls ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( ptc, *ls ), false );
    // symmetric calls
}

BOOST_AUTO_TEST_CASE( testPointTriangleIntersects )
{
    Point pta( 0.0, 0.0, 0.0 );
    Point ptb( 0.5, 0.0, 0.0 );
    Point ptc( 0.0, 0.0, 1.0 );
    Point ptd( 0.8, 0.2, 0.0 );
    Point pte( 2.8, 2.2, 0.0 );
    Triangle tri( Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(1.0, 1.0, 0.0) );
    // point on a vertex
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, tri ), false );
    // FIXME : should be false
    //    BOOST_CHECK_EQUAL( algorithm::intersects3D( pta, tri ), false );
    // point on a boundary
    BOOST_CHECK_EQUAL( algorithm::intersects( ptb, tri ), false );
    // FIXME : should be false
    //    BOOST_CHECK_EQUAL( algorithm::intersects3D( ptb, tri ), false );
    // point not inside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptc, tri ), false );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( ptc, tri ), false );
    // point inside the triangle
    BOOST_CHECK_EQUAL( algorithm::intersects( ptd, tri ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( ptd, tri ), true );
    // point outside the triangle
    BOOST_CHECK_EQUAL( algorithm::intersects( pte, tri ), false );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( pte, tri ), false );
}

BOOST_AUTO_TEST_CASE( testPointPolygonIntersects )
{
    Point pta( 0.0, 0.0 );
    Point ptb( 0.5, 0.0 );
    Point ptc( 0.0, 1.5 );
    Point ptd( 0.5, 0.5 );
    Point pte( 0.6, 0.6 );

    // a square
    std::vector<Point> ring;
    ring.push_back( Point(0.0, 0.0) );
    ring.push_back( Point(1.0, 0.0) );
    ring.push_back( Point(1.0, 1.0) );
    ring.push_back( Point(0.0, 1.0) );
    ring.push_back( Point(0.0, 0.0) );
    LineString ext(ring);
    Polygon poly( ext );

    // point on a vertex
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, poly ), false );
    // point on a boundary
    BOOST_CHECK_EQUAL( algorithm::intersects( ptb, poly ), false );
    // point outside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptc, poly ), false );
    // point inside
    BOOST_CHECK_EQUAL( algorithm::intersects( pte, poly ), true );
    // point inside
    // FIXME : should be false.
    // ! intersection with a polygon relies on a triangulation.
    // This 0.5, 0.5 point is on the edge of a triangle and thus return false
    //    BOOST_CHECK_EQUAL( algorithm::intersects( ptd, poly ), true );
}

BOOST_AUTO_TEST_CASE( testPointPolygon3DIntersects )
{
    Point pta( 0.0, 0.0 );
    Point ptb( 0.5, 0.0 );
    Point ptc( 0.0, 1.5 );
    Point ptd( 0.6, 0.6 );
    Point pte( 0.5, 0.5 );

    // a square
    std::vector<Point> ring;
    ring.push_back( Point(0.0, 0.0, 0.0) );
    ring.push_back( Point(1.0, 0.0, 0.0) );
    ring.push_back( Point(1.0, 1.0, 0.0) );
    ring.push_back( Point(0.0, 1.0, 0.0) );
    ring.push_back( Point(0.0, 0.0, 0.0) );
    LineString ext(ring);
    Polygon poly( ext );

    // point on a vertex
    BOOST_CHECK_EQUAL( algorithm::intersects( pta, poly ), false );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( pta, poly ), false );
    // point on a boundary
    BOOST_CHECK_EQUAL( algorithm::intersects( ptb, poly ), false );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( ptb, poly ), false );
    // point outside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptc, poly ), false );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( ptc, poly ), false );
    // point inside
    BOOST_CHECK_EQUAL( algorithm::intersects( ptd, poly ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects3D( ptd, poly ), true );
    // point inside and on a triangulation edge
    // FIXME: should be true
    //    BOOST_CHECK_EQUAL( algorithm::intersects( pte, poly ), true );
    //    BOOST_CHECK_EQUAL( algorithm::intersects3D( pte, poly ), true );
}

BOOST_AUTO_TEST_CASE( testLineStringLineStringIntersects )
{
    // 2D
    {
	std::auto_ptr<Geometry> ls = io::readWkt( "LINESTRING(0 0, 1 0, 1 1)" );
	std::auto_ptr<Geometry> lsb = io::readWkt( "LINESTRING(0 -1, 1 0.5, 1 4)" );
	std::auto_ptr<Geometry> lsc = io::readWkt( "LINESTRING(10 0, 11 0, 11 1)" );
	
	BOOST_CHECK_EQUAL( algorithm::intersects( *ls, *lsb ), true );
	BOOST_CHECK_EQUAL( algorithm::intersects( *lsc, *ls ), false );
    }
    // 3D
    {
	std::auto_ptr<Geometry> ls = io::readWkt( "LINESTRING(0 0 0, 1 0 0, 1 1 0)" );
	std::auto_ptr<Geometry> lsb = io::readWkt( "LINESTRING(0 0 0, 1 0 1, 1 4 0)" );
	std::auto_ptr<Geometry> lsc = io::readWkt( "LINESTRING(10 0 0, 11 0 0, 11 1 0)" );
	
	BOOST_CHECK_EQUAL( algorithm::intersects( *ls, *lsb ), true );
	BOOST_CHECK_EQUAL( algorithm::intersects( *lsc, *ls ), false );
    }
}

#if STILL_BUGGY
BOOST_AUTO_TEST_CASE( testLineStringTriangleIntersects )
{
    Triangle tri( Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(1.0, 1.0, 0.0) );
    std::auto_ptr<Geometry> ls = io::readWkt( "LINESTRING(0 0 0, 1 0 0, 1 1 0)" );
    std::auto_ptr<Geometry> lsb = io::readWkt( "LINESTRING(0 0 0, 1 0 1, 1 4 0)" );
    std::auto_ptr<Geometry> lsc = io::readWkt( "LINESTRING(10 0 0, 11 0 0, 11 1 0)" );
    
    BOOST_CHECK_EQUAL( algorithm::intersects( *ls, tri ), false );
    BOOST_CHECK_EQUAL( algorithm::intersects( *lsb, tri ), true );
    BOOST_CHECK_EQUAL( algorithm::intersects( *lsc, tri ), false );
}
#endif

BOOST_AUTO_TEST_CASE( intersects3DSolid )
{
	// the unit cube
	std::string gstr = "SOLID(("
		"((0 0 0,0 1 0,1 1 0,1 0 0,0 0 0))," // front face
		"((1 0 0,1 1 0,1 1 1,1 0 1,1 0 0))," // right face
		"((0 1 0,0 1 1,1 1 1,1 1 0,0 1 0))," // top face
		"((0 0 1,0 1 1,0 1 0,0 0 0,0 0 1))," // left face
		"((1 0 1,1 1 1,0 1 1,0 0 1,1 0 1))," // back face
		"((1 0 0,1 0 1,0 0 1,0 0 0,1 0 0))" // bottom face
		"))";

	std::auto_ptr<Geometry> g( io::readWkt( gstr ));

	const Solid& solid = static_cast<const Solid&>( *g );
	// intersection with a point
	{
		// point on a vertex
		BOOST_CHECK_EQUAL( algorithm::intersects3D( Point(0.0, 0.0, 0.0), solid ), false );
		// point on an edge
		BOOST_CHECK_EQUAL( algorithm::intersects3D( Point(0.0, 0.5, 0.0), solid ), false );
		// point outside the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( Point(1.5, 0.5, 0.5), solid ), false );
		// point inside the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( Point(0.5, 0.5, 0.5), solid ), true );
	}

	// intersection with a linestring
	{
		LineString lsa( Point(0.5, 0.5, 0.5), Point(0.7, 0.7, 0.7) );
		LineString lsb( Point(0.5, 0.5, 0.5), Point(1.5, 1.5, 1.5) );
		LineString lsc( Point(2.5, 2.5, 2.5), Point(1.5, 1.5, 1.5) );
		LineString lsd( Point(-1, 0.5, 0.5), Point(1.5, 0.5, 0.5) );
		LineString lse( Point(-1, 0.0, 0.0), Point(1.5, 0.0, 0.0) );
		// segment inside the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( lsa, solid ), true );
		// segment partly inside the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( lsb, solid ), true );
		// segment outside the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( lsc, solid ), false );
		// segment crossing the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( lsd, solid ), true );
		// segment crossing the volume on an edge
		// FIXME should be false ??
		//	BOOST_CHECK_EQUAL( algorithm::intersects3D( lse, solid ), false );
	}

	// intersection with a triangle
	{
		Triangle tri1( Point(0.5, 0.5, 0.5), Point(0.7, 0.7, 0.7), Point(0.2, 0.2, 0.2) );
		Triangle tri2( Point(-0.5, -0.5, -0.5), Point(0.7, 0.7, 0.7), Point(0.2, 0.2, 0.2) );
		Triangle tri3( Point(-0.5, -0.5, -0.5), Point(-0.7, -0.7, -0.7), Point(-0.2, -0.2, -0.2) );
		Triangle tri4( Point(-1, 0.5, 0.5), Point(1.5, 0.5, 0.5), Point(0, 0, 1.5) ); 
		Triangle tri5( Point(-1, 0.0, 0.0), Point(1.5, 0.0, 0.0), Point(0, 0, -1) ); 

		// triangle inside the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( tri1, solid ), true );
		// triangle partly inside the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( tri2, solid ), true );
		// triangle outside the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( tri3, solid ), false );
		// triangle crossing the volume
		BOOST_CHECK_EQUAL( algorithm::intersects3D( tri4, solid ), true );
		// triangle crossing the volume on an edge
		// FIXME should be false ??
		//		BOOST_CHECK_EQUAL( algorithm::intersects3D( tri5, solid ), false );
	}

	// intersection with another solid
	{
		Solid solidb = solid;
		Solid solidc = solid;
		Solid solidd = solid;

		CGAL::Vector_3<Kernel> tv( CGAL::Point_3<Kernel>( 0.0, 0.0, 0.0 ),
					   CGAL::Point_3<Kernel>( 2.0, 0.0, 0.0 ));
		transform::AffineTransform3<Kernel> t( CGAL::Aff_transformation_3<Kernel>( CGAL::Translation(),
											   tv ));
		// translate the solid
		t.transform( solidb );
		
		CGAL::Vector_3<Kernel> tv2( CGAL::Point_3<Kernel>( 0.0, 0.0, 0.0 ),
					    CGAL::Point_3<Kernel>( 1.0, 0.0, 0.0 ));
		transform::AffineTransform3<Kernel> t2( CGAL::Aff_transformation_3<Kernel>( CGAL::Translation(),
											    tv2 ));
		t2.transform( solidc );

		CGAL::Vector_3<Kernel> tv3( CGAL::Point_3<Kernel>( 0.0, 0.0, 0.0 ),
					    CGAL::Point_3<Kernel>( 0.5, 0.0, 0.0 ));
		transform::AffineTransform3<Kernel> t3( CGAL::Aff_transformation_3<Kernel>( CGAL::Translation(),
											    tv3 ));
		t2.transform( solidd );

		// a non-overlapping solid
		BOOST_CHECK_EQUAL( algorithm::intersects3D( solidb, solid ), false );
		// a touching solid (on the edge)
		// FIXME : should be false
		//		BOOST_CHECK_EQUAL( algorithm::intersects3D( solidc, solid ), false );
		// an overlapping solid
		BOOST_CHECK_EQUAL( algorithm::intersects3D( solidd, solid ), true );
	}
}

BOOST_AUTO_TEST_SUITE_END()
