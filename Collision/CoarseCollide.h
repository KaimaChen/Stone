/*
Desc: BVH（Bounding Volume Hierarchy）层次包围盒。本头文件是用二叉树来解决粗略的碰撞检测
注：
（1）因为这是模板类，因此实现要写在头文件里
（2）参考《游戏物理引擎开发》
*/

#ifndef COARSE_COLLIDE_H
#define COARSE_COLLIDE_H

#include "Rigidbody.h"
#include "Vector.h"

	class BoundingCircle {
	public:
		Vector2 mCenter;
		real mRadius;
	public:
		BoundingCircle(const Vector2 &center, real r) 
			: mCenter(center), mRadius(r) {}

		BoundingCircle(const BoundingCircle &one, const BoundingCircle &two) {
			Vector2 centerOffset = two.mCenter - one.mCenter;
			real distance = DistanceSquared(one.mCenter, two.mCenter);
			real radiusDiff = two.mRadius - one.mRadius; //it is diff not the sum

			//bigger one enclose the smaller one
			if(radiusDiff * radiusDiff >= distance) {
				if(one.mRadius > two.mRadius) {
					mCenter = one.mCenter;
					mRadius = one.mRadius;
				} else {
					mCenter = two.mCenter;
					mRadius = two.mRadius;
				}
			} else {
				distance = real_sqrt(distance);
				mRadius = (distance + one.mRadius + two.mRadius) * (0.5);
				mCenter = one.mCenter;
				if(distance > 0) {
					mCenter += centerOffset * ((mRadius - one.mRadius) / distance);
				}
			}
		}

		bool IsOverlaps(const BoundingCircle *other) const {
			real distanceSquared = DistanceSquared(mCenter, other->mCenter);
			return distanceSquared < (mRadius + other->mRadius) * (mRadius + other->mRadius);
		}

		real GetGrowth(const BoundingCircle &other) const {
			BoundingCircle newCircle(*this, other);
			return newCircle.mRadius * newCircle.mRadius - mRadius * mRadius;
		}
	};

	//@Class
	struct PotentialContact {
		Rigidbody *mBody[2];
	};

	//@Class
	template <typename BVClass>
	class BVHNode {
	public:
		BVHNode *mParent;
		BVHNode *mChildren[2];
		BVClass mVolume; //the BV should encompassing all the children
		Rigidbody *mBody; //only the leaf has rigidbody
	public:
		BVHNode(BVHNode *parent, const BVClass &volume, Rigidbody *body = nullptr)
		: mParent(parent), mVolume(volume), mBody(body)
		{ mChildren[0] = mChildren[1] = nullptr; }
		~BVHNode();
	public:
		bool IsLeaf() const;
		unsigned GetPotentialContacts(PotentialContact *contacts) const;
		void Insert(Rigidbody *body, const BVClass &volume);
		void Draw() const;
	protected:
		bool IsOverlaps(const BVHNode<BVClass> *other) const;

		unsigned GetPotentialContactsWith(
			const BVHNode<BVClass> *other,
			PotentialContact *contacts,
			bool descend) const;

		void RecalculateBoundingVolume();
	};

	template<typename BVClass>
	bool BVHNode<BVClass>::IsLeaf() const {
		return (mBody != nullptr);
	}

	template<typename BVClass>
	unsigned BVHNode<BVClass>::GetPotentialContacts(PotentialContact *contacts) const {
		if(IsLeaf()) return 0;

		return mChildren[0]->GetPotentialContactsWith(mChildren[1], contacts);
	}

	template<typename BVClass>
	void BVHNode<BVClass>::Insert(Rigidbody *body, const BVClass &volume) {
		if(IsLeaf()) {
			mChildren[0] = new BVHNode<BVClass>(this, mVolume, mBody);
			mChildren[1] = new BVHNode<BVClass>(this, volume, body);

			mBody = nullptr;

			RecalculateBoundingVolume();
		} 
		//insert into the smaller growth node
		else {
			real firstGrowth = mChildren[0]->mVolume.GetGrowth(volume);
			real secondGrowth = mChildren[1]->mVolume.GetGrowth(volume);
			if(firstGrowth < secondGrowth) {
				mChildren[0]->Insert(body, volume);
			} else {
				mChildren[1]->Insert(body, volume);
			}
		}
	}

	template<typename BVClass>
	BVHNode<BVClass>::~BVHNode() {
		if(mParent) {
			BVHNode<BVClass> *sibling;
			if(mParent->mChildren[0] == this) 
				sibling = mParent->mChildren[1];
			else 
				sibling = mParent->mChildren[0];

			mParent->mVolume = sibling->mVolume;
			mParent->mBody = sibling->mBody;
			mParent->mChildren[0] = sibling->mChildren[0];
			mParent->mChildren[1] = sibling->mChildren[1];

			sibling->mParent = nullptr;
			sibling->mBody = nullptr;
			sibling->mChildren[0] = nullptr;
			sibling->mChildren[1] = nullptr;
			delete sibling;

			mParent->RecalculateBoundingVolume();
		}

		if(mChildren[0]) {
			mChildren[0]->mParent = nullptr;
			delete mChildren[0];
		}

		if(mChildren[1]) {
			mChildren[1]->mParent = nullptr;
			delete mChildren[1];
		}
	}

	template<typename BVClass>
	bool BVHNode<BVClass>::IsOverlaps(const BVHNode<BVClass> *other) const {
		return mVolume.IsOverlaps(other->mVolume);
	}

	template<typename BVClass>
	void BVHNode<BVClass>::RecalculateBoundingVolume() {
		if(IsLeaf()) return;

		mVolume = BVClass(mChildren[0]->mVolume, mChildren[1]->mVolume);

		if(mParent) mParent->RecalculateBoundingVolume();
	}

	template<typename BVClass>
	unsigned BVHNode<BVClass>::GetPotentialContactsWith(
		const BVHNode<BVClass> *other, 
		PotentialContact *contacts,
		bool descend = true) const {
		
		cout << "GetPotentialContactsWith" << endl;
		/*
		if(!IsOverlaps(other)) return 0;

		//if overlaps
		if(IsLeaf() && other->IsLeaf()) {
			contacts->mBody[0] = mBody;
			contacts->mBody[1] = other->mBody;
			return 1;
		}

		unsigned count = 0;
		
		bool isOurSizeBigger = mVolume.GetSize() >= other->mVolume.GetSize();
		if(other->IsLeaf() || (!IsLeaf() && isOurSizeBigger)) {
			count = mChildren[0]->GetPotentialContactsWith(other, contacts);
			count += mChildren[1]->GetPotentialContactsWith(other, contacts + count);
		} else {
			count = GetPotentialContactsWith(other->mChildren[0], contacts);
			count += GetPotentialContactsWith(other->mChildren[1], contacts + count);
		}
		*/
		/*
		//now can't be both leaf
		if(other->IsLeaf()) {
			count += mChildren[0]->GetPotentialContactsWith(other, contacts);
			count += mChildren[1]->GetPotentialContactsWith(other, contacts + count);
		} else if(IsLeaf()) {
			count += GetPotentialContactsWith(other->mChildren[0], contacts);
			count += GetPotentialContactsWith(other->mChildren[1], contacts + count);
		} else {
			count += mChildren[0]->GetPotentialContactsWith(other, contacts);
			count += mChildren[1]->GetPotentialContactsWith(other, contacts + count);
			count += GetPotentialContactsWith(other->mChildren[0], contacts + count);
			count += GetPotentialContactsWith(other->mChildren[1], contacts + count);
		}
		*/

		unsigned count = 0;
		if((descend) && (!IsLeaf())) {
			count += mChildren[0]->GetPotentialContactsWith(mChildren[1], contacts, descend);
		}

		if((descend) && !other->IsLeaf()) {
			count += other->mChildren[0]->GetPotentialContactsWith(other->mChildren[1], contacts + count, descend);
		}

		if(!IsOverlaps(other))
			return 0;

		if(IsLeaf() && (other->IsLeaf())) {
			contacts->mBody[0] = mBody;
			contacts->mBody[1] = other->mBody;
			return 1;
		}

		if(other->IsLeaf() || (!IsLeaf() && mVolume.GetSize() >= other->mVolume.GetSize())) {
			count += mChildren[0]->GetPotentialContactsWith(other, contacts + count, false);
			count += mChildren[1]->GetPotentialContactsWith(other, contacts + count, false);
		} else {
			count += GetPotentialContactsWith(other->mChildren[0], contacts + count, false);
			count += GetPotentialContactsWith(other->mChildren[1], contacts + count, false);
		}


		return count;
	}

	template<typename BVClass>
	void BVHNode<BVClass>::Draw() const {
		mVolume.Draw();
		if(mChildren[0]) {
			mChildren[0]->mVolume.Draw();
			mChildren[0]->Draw();
		}
		if(mChildren[1]) {
			mChildren[1]->mVolume.Draw();
			mChildren[1]->Draw();
		}
	}

#endif // COARSE_COLLIDE_H
