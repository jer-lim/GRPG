//MATTSCAN: LIKELIHOOD OF MEMORY LEAK: VERY UNLIKELY

#ifndef _DESTINATION_H               // Prevent multiple definitions if this 
#define _DESTINATION_H               // file is included in more than one place

class Destination
{
public:
	virtual ~Destination() {}
    // Get the x location of the destination
	virtual float getX() = 0;
    // Get the y location fo the destination
	virtual float getY() = 0;
	// Get the vector that stores the destination
	virtual D3DXVECTOR2 getVector() = 0;
	// Release this destination if it is unlikely to be used otherwise
	virtual void release() = 0;
};

class Point : public Destination
{
private:
	D3DXVECTOR2 pointDetails;
public:
	Point(float x, float y)
	{
		pointDetails = VECTOR2(x, y);
	}

	Point(D3DXVECTOR2 thePoint)
	{
		pointDetails = thePoint;
	}

	virtual float getX()
	{
		return pointDetails.x;
	}

	virtual float getY()
	{
		return pointDetails.y;
	}

	virtual D3DXVECTOR2 getVector()
	{
		return pointDetails;
	}

	// Set the point using a vector2
	void setPoint(D3DXVECTOR2 p)
	{
		pointDetails = p;
	}

	void setX(float theX)
	{
		pointDetails.x = theX;
	}

	void setY(float theY)
	{
		pointDetails.y = theY;
	}

	//Release the point
	virtual void release()
	{
		delete this;
	}
};

#endif