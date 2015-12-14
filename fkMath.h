/**
* @file fDxBase.h
* @brief ���w�A�􉽊w�֘A�̎��� Point Rect����������
* @author �g�T
* @date 2014/04/23
*/

namespace fk
{
	/**
	* ���W�̍\����
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
	* �傫���̍\����
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
	*�l�p�̍\���� Point��Size�Ŕ����ɋ������Ⴄ�̂Œ���
	*/
	template <class T>
	struct Rect
	{
		T x;
		T y;
		T width;
		T height;
		/**
		*�l�p���쐬����
		* @param x_ �����x���W
		* @param y_ �����y���W
		* @param width_ ����
		* @param height_ �c��
		*/
		Rect(const T &x_, const T &y_, const T &width_, const T &height_) :x(x_), y(y_), width(width_), height(height_){}
		Rect(const Point &position, const Size &size) :x(position.x), y(position.y), width(size.width), height(size.height){}
		Rect() :x(0), y(0), width(0), height(0){}
	};
	typedef Rect<int>  IRect;
	typedef Rect<double> DRect;
}
