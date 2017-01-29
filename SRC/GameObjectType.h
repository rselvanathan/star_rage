/*
This creates a unique hash key for a game object/entity to be used in many places such as collisions

Reference Directly from previous 2nd Year project Asteroids

Author : Dr. Chris Child
*/

#ifndef __GAMEOBJECTTYPE__H__
#define __GAMEOBJECTTYPE__H__

class GameObjectType
{
public:
	explicit GameObjectType(char const * const pTypeName)
		: mTypeName(pTypeName), mTypeID(HashName(pTypeName))
	{}

	unsigned long GetTypeID() const { return mTypeID; }
	char const * const GetTypeName() const { return mTypeName; }
	/* Added By Romesh Selvanathan 02/03/2012 */
	void SetTypeName(char const * const pTypeName) {mTypeName = pTypeName; mTypeID = HashName(pTypeName);}

	bool operator< (GameObjectType const & o) const { return (GetTypeID() < o.GetTypeID()); }
	bool operator== (GameObjectType const & o) const { return (GetTypeID() == o.GetTypeID()); }
	bool operator!= (GameObjectType const & o) const { return (GetTypeID() != o.GetTypeID()); }

	static unsigned long HashName(char const * const pTypeName);

private:

	unsigned long mTypeID;
	char const * mTypeName;
};

#endif