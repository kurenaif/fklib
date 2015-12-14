/**
* @file fDxBase.h
* @brief 数学、幾何学関連の実装 Point Rect等もここで
* @author 紅狼
* @date 2014/04/23
*/

namespace fk
{
	/**
	* 座標の構造体
	*/
	template <class T>
	struct Point
	{
		T x;
		T y;
		Point(T x_,T y_) :x(x_), y(y_){}
		Point() :x(0), y(0){}
	};
	typedef Point<int>		IPoint;
	typedef Point<double>	DPoint;

	/**
	* 大きさの構造体
	*/
	template <class T>
	struct Size
	{
		T width;
		T height;
		Size(const T &width_, const T &height_) :width(width_), height(height_){}
		Size() :width(0), height(0){}
	};

	/**
	*四角の構造体 PointとSizeで微妙に挙動が違うので注意
	*/
	template <class T>
	struct Rect
	{
		T x;
		T y;
		T width;
		T height;
		/**
		*四角を作成する
		* @param x_ 左上のx座標
		* @param y_ 左上のy座標
		* @param width_ 横幅
		* @param height_ 縦幅
		*/
		Rect(const T &x_, const T &y_, const T &width_, const T &height_) :x(x_), y(y_), width(width_), height(height_){}
		Rect(const Point &position, const Size &size) :x(position.x), y(position.y), width(size.width), height(size.height){}
		Rect() :x(0), y(0), width(0), height(0){}
	};
	typedef Rect<int>  IRect;
	typedef Rect<double> DRect;
}
