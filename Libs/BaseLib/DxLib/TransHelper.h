/*!
@file TransHelper.h
@brief �Փ˔���A��ԏ����p���[�e�B���e�B
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	Lerp ��ԏ����v�Z�\����
	//--------------------------------------------------------------------------------------
	struct Lerp{
		enum rate{
			Linear,	//1���֐��i���`�j
			Easein,	//2���֐��������㏸
			EaseOut,//2���֐��Ōオ�������
			Cube,	//3���֐�
			Cos		//�R�T�C��
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T�^�̕�Ԍv�Z���s��
		@tparam	T	�v�Z����^
		@param[in]	Start	�J�n�l
		@param[in]	End	�I���l
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[in]	NowTime	���݂̎���
		@param[in]	r	�v�Z���@
		@return	T�^�̌v�Z����
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T CalculateLerp(const T &Start, const T &End,
		float StartTime, float EndTime, float NowTime, rate r){
			float t = (NowTime - StartTime) / (EndTime - StartTime);
			if (t < 0.0f){
				t = 0;
			}
			if (t > 1.0f){
				t = 1.0f;
			}
			float cal_rate = t;   // 1���֐���Ԓl�ɕϊ�(�f�t�H���g)
			switch (r){
			case Linear:
				cal_rate = t;
				break;
			case Easein:
				cal_rate = t * t;
				break;
			case EaseOut:
				cal_rate = t * (2.0f - t);
				break;
			case Cube:
				cal_rate = t * t * (3.0f - 2.0f * t);
				break;
			case Cos:
				cal_rate = (1.0f - cos(t * XM_PI)) / 2.0f;
				break;
			default:
				break;
			}
			return Start * (1.0f - cal_rate) + End * cal_rate;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�Փ˔���{�����[���̊��N���X
	//--------------------------------------------------------------------------------------
	struct CollisionVolume{
		CollisionVolume(){}
		virtual ~CollisionVolume(){}
	};
	//--------------------------------------------------------------------------------------
	///	OBB�{�����[�����E
	//--------------------------------------------------------------------------------------
	struct OBB : public CollisionVolume{
		Vector3 m_Center;     ///< ���S�_�̍��W
		Vector3 m_Rot[3];  ///< XYZ �̊e���W���̌X����\�������x�N�g��
		Vector3 m_Size;     ///< OBB �̊e���W���ɉ����������̔����i���S�_����ʂ܂ł̒����j
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�쐬���ꂽ�T�C�Y�ƕϊ��s�񂩂�AOBB����蒼��
		@param[in]	Size	�쐬�T�C�Y
		@param[in]	Matrix	�ϊ��s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateOBB(const Vector3& Size,const Matrix4X4& Matrix){
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			Vector3 VecX(Matrix._11,Matrix._12,Matrix._13);
			Vector3 VecY(Matrix._21,Matrix._22,Matrix._23);
			Vector3 VecZ(Matrix._31,Matrix._32,Matrix._33);
			m_Size = Vector3(
				Size.x * VecX.Length(),
				Size.y * VecY.Length(),
				Size.z * VecZ.Length()
				);
			m_Size *= 0.5f;
			//��]�𓾂�
			m_Rot[0] = Vector3EX::Normalize(VecX);
			m_Rot[1] = Vector3EX::Normalize(VecY);
			m_Rot[2] = Vector3EX::Normalize(VecZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		OBB(){
			//�f�t�H���g��1��1.0��OBB
			Vector3 SizeVec(1.0f,1.0f,1.0f);
			//�s��̓A�C�f���e�B�e�B
			Matrix4X4 Matrix;
			CreateOBB(SizeVec,Matrix);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Size	�T�C�Y
		@param[in]	Rot	��]�x�N�g��
		@param[in]	Center	���S�ʒu
		*/
		//--------------------------------------------------------------------------------------
		OBB(const Vector3& Size,const Vector3& Rot,const Vector3& Center):
			m_Center(Center),
			m_Size(Size)
		{
			m_Size *= 0.5f;
			//��]�𓾂�
			Vector3 LocaRot = Rot;
			LocaRot.Normalize();
			Matrix4X4 mRot;
			mRot.Identity();
			mRot.RotationRollPitchYawFromVector(LocaRot);
			m_Rot[0] = Vector3(mRot._11,mRot._12,mRot._13);
			m_Rot[1] = Vector3(mRot._21,mRot._22,mRot._23);
			m_Rot[2] = Vector3(mRot._31,mRot._32,mRot._33);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Size	�T�C�Y
		@param[in]	Qt	��]�N�I�[�^�j�I��
		@param[in]	Center	���S�ʒu
		*/
		//--------------------------------------------------------------------------------------
		OBB(const Vector3& Size,const Quaternion& Qt,const Vector3& Center):
			m_Center(Center),
			m_Size(Size)
		{
			m_Size *= 0.5f;
			//��]�𓾂�
			Quaternion LocalQt = Qt;
			//�N�I�[�^�j�I���𐳋K��
			LocalQt.Normalize();
			//�N�I�[�^�j�I�������]�s����쐬
			Matrix4X4 mRot;
			mRot.Identity();
			mRot.RotationQuaternion(LocalQt);
			m_Rot[0] = Vector3(mRot._11,mRot._12,mRot._13);
			m_Rot[1] = Vector3(mRot._21,mRot._22,mRot._23);
			m_Rot[2] = Vector3(mRot._31,mRot._32,mRot._33);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^�i�쐬���ꂽ�T�C�Y�ƕϊ��s�񂩂�AOBB���쐬�j
		@param[in]	Size	�T�C�Y
		@param[in]	Matrix	�ϊ��s��
		*/
		//--------------------------------------------------------------------------------------
		OBB(const Vector3& Size,const Matrix4X4& Matrix)
		{
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			Vector3 VecX(Matrix._11,Matrix._12,Matrix._13);
			Vector3 VecY(Matrix._21,Matrix._22,Matrix._23);
			Vector3 VecZ(Matrix._31,Matrix._32,Matrix._33);
			m_Size = Vector3(
				Size.x * VecX.Length(),
				Size.y * VecY.Length(),
				Size.z * VecZ.Length()
				);
			m_Size *= 0.5f;
			//��]�𓾂�
			m_Rot[0] = Vector3EX::Normalize(VecX);
			m_Rot[1] = Vector3EX::Normalize(VecY);
			m_Rot[2] = Vector3EX::Normalize(VecZ);

		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��]�s��𓾂�
		@return	��]�s��
		*/
		//--------------------------------------------------------------------------------------
		Matrix4X4 GetRotMatrix(){
			Matrix4X4 ret;
			ret._11 = m_Rot[0].x;
			ret._12 = m_Rot[0].y;
			ret._13 = m_Rot[0].z;
			ret._21 = m_Rot[1].x;
			ret._22 = m_Rot[1].y;
			ret._23 = m_Rot[1].z;
			ret._31 = m_Rot[2].x;
			ret._32 = m_Rot[2].y;
			ret._33 = m_Rot[2].z;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ԑ傫���ӂ̒����𓾂�
		@return	��ԑ傫���ӂ̒���
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSize(){
			float ret = m_Size.x;
			if(ret < m_Size.y){
				ret = m_Size.y;
			}
			if(ret < m_Size.z){
				ret = m_Size.z;
			}
			return ret * 2.0f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ԏ������ӂ̒����𓾂�
		@return	��ԏ������ӂ̒���
		*/
		//--------------------------------------------------------------------------------------
		float GetMinSize(){
			float ret = m_Size.x;
			if(ret > m_Size.y){
				ret = m_Size.y;
			}
			if(ret > m_Size.z){
				ret = m_Size.z;
			}
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�x�N�g���Ɉ�ԋ߂���]�𓾂�
		@param[in]	Check	�`�F�b�N����x�N�g��
		@param[in]	Epsilon	�덷
		@param[out]	Ret	�߂����x�N�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetNearNormalRot(const Vector3& Check, float Epsilon,Vector3& Ret){
			for (int i = 0; i < 3; i++){
				if (Vector3EX::NearEqual(Check, m_Rot[i], Epsilon)){
					Ret = m_Rot[i];
					return;
				}
				if (Vector3EX::NearEqual(Check, -m_Rot[i], Epsilon)){
					Ret = -m_Rot[i];
					return;
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB����]���ĂȂ����ǂ����𒲂ׂ�iAABB�Ƃ��Ďg���邩�ǂ����𒲂ׂ���j
		@return	��]���ĂȂ����true
		*/
		//--------------------------------------------------------------------------------------
		bool IsRotIdentity() const{
			if (m_Rot[0] == Vector3(1.0f, 0.0f, 0.0f) && m_Rot[1] == Vector3(0.0f, 1.0f, 0.0f) && m_Rot[2] == Vector3(0.0f, 0.0f, 1.0f)){
				return true;
			}
			return false;
		}
	};

	//--------------------------------------------------------------------------------------
	///	AABB�{�����[�����E
	//--------------------------------------------------------------------------------------
	struct AABB : public CollisionVolume{
		Vector3 m_Min;     ///< �������ق��̍��W
		Vector3 m_Max;     ///< �傫���ق��̍��W
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		AABB(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Min	�������ق��̍��W
		@param[in]	Max	�傫���ق��̍��W
		*/
		//--------------------------------------------------------------------------------------
		AABB(const Vector3& Min,const Vector3& Max)
			:m_Min(Min),m_Max(Max){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Center	���S
		@param[in]	XSize	X�����T�C�Y
		@param[in]	YSize	Y�����T�C�Y
		@param[in]	ZSize	Z�����T�C�Y
		*/
		//--------------------------------------------------------------------------------------
		AABB(const Vector3& Center,float XSize,float YSize,float ZSize){
			float xh = XSize / 2.0f;
			float yh = YSize / 2.0f;
			float zh = ZSize / 2.0f;

			m_Min.x = Center.x - xh;
			m_Max.x = Center.x + xh;

			m_Min.y = Center.y - yh;
			m_Max.y = Center.y + yh;

			m_Min.z = Center.z - zh;
			m_Max.z = Center.z + zh;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�𓾂�
		@param[out]	Pos	���S����������x�N�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetCenter(Vector3& Pos){
			Pos.x = (m_Min.x + m_Max.x) / 2.0f;
			Pos.y = (m_Min.y + m_Max.y) / 2.0f;
			Pos.z = (m_Min.z + m_Max.z) / 2.0f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�𓾂�
		@return	���S�̃x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetCenter(){
			Vector3 ret;
			ret.x = (m_Min.x + m_Max.x) / 2.0f;
			ret.y = (m_Min.y + m_Max.y) / 2.0f;
			ret.z = (m_Min.z + m_Max.z) / 2.0f;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړ�����
		@param[in]	MoveVec	�ړ��x�N�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Move(const Vector3& MoveVec){
			m_Min += MoveVec;
			m_Max += MoveVec;
		}
	};


	//--------------------------------------------------------------------------------------
	///	���{�����[�����E
	//--------------------------------------------------------------------------------------
	struct SPHERE : public CollisionVolume
	{
		Vector3 m_Center;	///< ���S�_�̍��W
		float m_Radius;			///< ���a
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SPHERE(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Center	���S
		@param[in]	Radius	���a
		*/
		//--------------------------------------------------------------------------------------
		SPHERE(const Vector3& Center,float Radius):
			m_Center(Center),
			m_Radius(Radius){}
	};

	//--------------------------------------------------------------------------------------
	///	���z��f�[�^
	//--------------------------------------------------------------------------------------
	struct SPHERE_ARR : public CollisionVolume{
		vector<SPHERE> m_SphereArr;
	};

	//--------------------------------------------------------------------------------------
	///	����
	//--------------------------------------------------------------------------------------
	struct PLANE : public CollisionVolume{
		Vector3 m_Normal;	///< �@��
		float m_DotValue;	///< ���ϒl
		//--------------------------------------------------------------------------------------
		/*!
		@brief	3�̓_���畽�ʂ��쐬
		@param[in]	PointA	�_A
		@param[in]	PointB	�_B
		@param[in]	PointC	�_C
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Set(const Vector3& PointA,const Vector3& PointB,const Vector3& PointC){
			m_Normal = Vector3EX::Normalize(Vector3EX::Cross(PointB - PointA,PointC - PointA));
			m_DotValue = Vector3EX::Dot(m_Normal,PointA);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		PLANE(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^�i3�̓_���畽�ʂ��쐬�j
		@param[in]	PointA	�_A
		@param[in]	PointB	�_B
		@param[in]	PointC	�_C
		*/
		//--------------------------------------------------------------------------------------
		PLANE(const Vector3& PointA,const Vector3& PointB,const Vector3& PointC){
			Set(PointA,PointB,PointC);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^�iPlane�^���畽�ʂ��쐬�j
		@param[in]	plane	Plane�^�̒l
		*/
		//--------------------------------------------------------------------------------------
		PLANE(const Plane& plane){
			m_Normal.x = plane.x;
			m_Normal.y = plane.x;
			m_Normal.z = plane.z;
			m_DotValue = -plane.w;
		}
	};


	//--------------------------------------------------------------------------------------
	///	�J�v�Z���{�����[�����E
	//--------------------------------------------------------------------------------------
	struct CAPSULE  : public CollisionVolume{
		float m_Radius;			///< ���a
		Vector3 m_PointA;		///< ���ԕ������̊J�n�_
		Vector3 m_PointB;		///< ���ԕ������̏I���_
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE():
			m_Radius(0.5f),
			m_PointA(0,-0.5f,0),
			m_PointB(0,0.5f,0)
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	Radius	���a
		@param[in]	PointA	���ԕ������̊J�n�_
		@param[in]	PointB	���ԕ������̏I���_
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE(float Radius,const Vector3& PointA,const Vector3& PointB):
			m_Radius(Radius),
			m_PointA(PointA),
			m_PointB(PointB)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	MakedRadius	�쐬���̔��a
		@param[in]	MakedPointA	�쐬���̒��ԕ������̊J�n�_
		@param[in]	MakedPointB	�쐬���̒��ԕ������̏I���_
		@param[in]	Matrix	�ϊ��s��
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE(float MakedRadius,const Vector3& MakedPointA,const Vector3& MakedPointB,
			const Matrix4X4& Matrix):
			m_Radius(MakedRadius),
			m_PointA(MakedPointA),
			m_PointB(MakedPointB)
		{
			m_PointA.Transform(Matrix);
			m_PointB.Transform(Matrix);
			//�X�P�[�����O�̂�1�����Ōv�Z
			Vector3 Scale = Matrix.ScaleInMatrix();
			m_Radius *= Scale.x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�_�𓾂�
		@return	���S�_
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetCenter() const{
			return m_PointA + ((m_PointB - m_PointA) * 0.5f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�_��ݒ肷��
		@param[in]	Center	���S�_
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetCenter(const Vector3& Center){
			Vector3 CenterToPointA = ((m_PointA - m_PointB) * 0.5f);
			Vector3 CenterToPointB = ((m_PointB - m_PointA) * 0.5f);
			m_PointA = Center + CenterToPointA;
			m_PointB = Center + CenterToPointB;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�c�̔��a�𓾂�
		@return	�c�̔��a
		*/
		//--------------------------------------------------------------------------------------
		float GetHeightRadius()const{
			float PointLen = Vector3EX::Length(m_PointB - m_PointA) * 0.5f;
			PointLen += m_Radius;
			return PointLen;
		}
	};



	//--------------------------------------------------------------------------------------
	///	�Փ˔���p�̃��[�e�B���e�B�\����
	//--------------------------------------------------------------------------------------
	struct HitTest{
		HitTest(){}
		~HitTest(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	float�^�̃N�����v
		@param[in]	n	�N�����v����l
		@param[in]	min	�ŏ��l
		@param[in]	max	�ő�l
		@return	�N�����v���ꂽfloat
		*/
		//--------------------------------------------------------------------------------------
		static float floatClamp(float n,float min,float max){
			if(n < min) return min;
			if(n > max) return max;
			return n;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_���ʂ̗����ɂ��邩�ǂ�������
		@param[in]	p	���؂���_
		@param[in]	pl	��
		@return	�����Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		static bool InsidePtPlane(const Vector3& p, const PLANE& pl){
			float dist = Vector3EX::Dot(p,pl.m_Normal) - pl.m_DotValue;
			return dist < 0.0f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������m�̍ŋߐړ_���v�Z����i�J�v�Z�����m�̔���Ɏg�p�j
		@param[in]	p1	�J�v�Z��1�̐����J�n�_
		@param[in]	q1	�J�v�Z��1�̐����I���_
		@param[in]	p2	�J�v�Z��2�̐����J�n�_
		@param[in]	q2	�J�v�Z��2�̐����I���_
		@param[out]	s	����1�̍ŋߐړ_�̃p�����[�^
		@param[out]	t	����2�̍ŋߐړ_�̃p�����[�^
		@param[out]	c1	�ŋߐړ_1
		@param[out]	c2	�ŋߐړ_2
		@return	�ŋߐړ_c1,c2���v�Z���A���̃p�����[�^s,t���v�Z���������m�̋����̕�����Ԃ�
		*/
		//--------------------------------------------------------------------------------------
		static float ClosestPtSegmentSegment(
			const Vector3& p1,const Vector3& q1,const Vector3& p2,const Vector3& q2,
			float& s,float& t,Vector3& c1,Vector3& c2){
			const float EPSILON = 1.175494e-37f;
			Vector3 d1 = q1 - p1;
			Vector3 d2 = q2 - p2;
			Vector3 r = p1 - p2;
			float a = Vector3EX::Dot(d1,d1);
			float e = Vector3EX::Dot(d2,d2);
			float f = Vector3EX::Dot(d2,r);

			if(f <= EPSILON && e <= EPSILON){
				s = t = 0.0f;
				c1 = p1;
				c2 = p2;
				return Vector3EX::Dot(c1 - c2,c1 - c2);
			}
			if(a <= EPSILON){
				s = 0.0f;
				t = f / e;
				t = floatClamp(t,0.0f,1.0f);
			}
			else{
				float c = Vector3EX::Dot(d1,r);
				if(e <= EPSILON){
					t = 0.0f;
					s = floatClamp(-c / a,0.0f,1.0f);
				}
				else{
					float b = Vector3EX::Dot(d1,d2);
					float denom = a * e - b * b;
					if(denom != 0.0f){
						s = floatClamp((b * f - c * e) / denom,0.0f,1.0f);
					}
					else{
						s = 0.0f;
					}
					t = (b * s + f) / e;
					if(t < 0.0f){
						t = 0.0f;
						s = floatClamp(-c / a,0.0f,1.0f);
					}
					else if(t > 1.0f){
						t = 1.0f;
						s = floatClamp((b - c) / a,0.0f,1.0f);
					}
				}
			}
			c1 = p1 + d1 * s;
			c2 = p2 + d2 * t;
			return Vector3EX::Dot(c1 - c2,c1 - c2);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_c�Ɛ���ab�̋����̕�����Ԃ�
		@param[in]	a	�����J�n�_
		@param[in]	b	�����I���_
		@param[in]	c	�v�Z�_
		@return	�_c�Ɛ���ab�̋����̕���
		*/
		//--------------------------------------------------------------------------------------
		static float SqDistPointSegment(const Vector3& a, const Vector3& b, const Vector3& c){
			Vector3 ab = b - a;
			Vector3 ac = c - a;
			Vector3 bc = c - b;
			float e = Vector3EX::Dot(ac,ab);
			if(e <= 0.0f){
				return Vector3EX::Dot(ac,ac);
			}
			float f = Vector3EX::Dot(ab,ab);
			if(e >= f){
				return Vector3EX::Dot(bc,bc);
			}
			return Vector3EX::Dot(ac,ac) - e * e / f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_c�Ɛ���ab�̍ŋߐړ_��Ԃ�
		@param[in]	c	���ƂɂȂ�_
		@param[in]	a	�����J�n�_
		@param[in]	b	�����I���_
		@param[out]	t	�ŋߐړ_�̐�����̈ʒu�̊����̖߂�l
		@param[out]	d	�ŋߐړ_�̖߂�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosetPtPointSegment(const Vector3& c,
			const Vector3& a,const Vector3& b,
			float& t,Vector3& d){
			Vector3 ab = b - a;
			t = Vector3EX::Dot(c - a,ab) / Vector3EX::Dot(ab,ab);
			if(t < 0.0f){
				t = 0.0f;
			}
			if(t > 1.0f){
				t = 1.0f;
			}
			d = a + (ab * t);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ƃJ�v�Z���Ƃ̏Փ˔���
		@param[in]	sp	��
		@param[in]	cap	�J�v�Z��
		@param[out]	d	�ŋߐړ_�̖߂�l
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_CAPSULE(const SPHERE& sp, const CAPSULE& cap, Vector3& d){
			//�܂��ŋߐړ_��ݒ�
			float t;
			Vector3 ClosetPoint;
			ClosetPtPointSegment(sp.m_Center, cap.m_PointA, cap.m_PointB, t, ClosetPoint);
			//ClosetPoint�́A�J�v�Z��������̓_�ł���
			Vector3 Normal = sp.m_Center - ClosetPoint;
			Normal.Normalize();
			Normal *= cap.m_Radius;
			//�ŋߐړ_��ݒ�
			d = ClosetPoint + Normal;
			//�Փ˂��Ă��邩����
			float dist2 = SqDistPointSegment(cap.m_PointA,cap.m_PointB,sp.m_Center);
			float radius = sp.m_Radius + cap.m_Radius;
			return dist2 <= radius * radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z���ƃJ�v�Z���Ƃ̏Փ˔���
		@param[in]	cap1	�J�v�Z��1
		@param[in]	cap2	�J�v�Z��2
		@param[out]	retvec1	�J�v�Z��1�̍ŋߐړ_��Ԃ��Q��
		@param[out]	retvec2	�J�v�Z��2�̍ŋߐړ_��Ԃ��Q��
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_CAPSULE(
			const CAPSULE& cap1,
			const CAPSULE& cap2,
			Vector3& retvec1,
			Vector3& retvec2){
			float s,t;
			float dist2 = ClosestPtSegmentSegment(cap1.m_PointA,cap1.m_PointB,cap2.m_PointA,cap2.m_PointB,
				s,t,retvec1,retvec2);
			float radius = cap1.m_Radius + cap2.m_Radius;
			return dist2 <= radius * radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	point���猩��OBB�̍ŋߐړ_�𓾂�
		@param[in]	point	��_
		@param[in]	obb	OBB
		@param[out]	retvec	�ŋߐړ_��Ԃ��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointOBB(const Vector3& point, const OBB& obb, Vector3& retvec){
			Vector3 d = point - obb.m_Center;
			retvec = obb.m_Center;
			float dist;
			for(int i = 0; i < 3; i++)
			{
				dist = Vector3EX::Dot(d,obb.m_Rot[i]);
				if(dist > obb.m_Size[i])
				{
					dist = obb.m_Size[i];
				}
				if(dist < -obb.m_Size[i])
				{
					dist = -obb.m_Size[i];
				}
				retvec +=  dist * obb.m_Rot[i];
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Ƌ��Ƃ̏Փ˔���
		@param[in]	sp1	��1
		@param[in]	sp2	��2
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_SPHERE(const SPHERE& sp1, const SPHERE& sp2){
			Vector3 d = sp1.m_Center - sp2.m_Center;
			float dist2 = Vector3EX::Dot(d,d);
			float radiussum = sp1.m_Radius + sp2.m_Radius;
			return dist2 <= radiussum * radiussum;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Ƌ��z��Ƃ̏Փ˔���
		@param[in]	sp1	��1
		@param[in]	sparr	���z��
		@param[out]	HitIndex	�Փ˂��Ă����ꍇ�̔z��̃C���f�b�N�X
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_SPHERE_ARR(const SPHERE& sp1, const SPHERE_ARR& sparr, size_t& HitIndex){
			size_t sz = sparr.m_SphereArr.size();
			for(size_t i = 0;i < sz;i++){
				if(SPHERE_SPHERE(sp1,sparr.m_SphereArr[i])){
					HitIndex = i;
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���z��Ƌ��z��Ƃ̏Փ˔���
		@param[in]	srcarr	���z��src
		@param[in]	destarr	���z��dest
		@param[out]	SrcHitIndex	�Փ˂��Ă����ꍇ��Src�z��̃C���f�b�N�X
		@param[out]	DestHitIndex	�Փ˂��Ă����ꍇ��Dest�z��̃C���f�b�N�X
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_ARR_SPHERE_ARR(const SPHERE_ARR& srcarr, const SPHERE_ARR& destarr,
			size_t& SrcHitIndex,size_t& DestHitIndex){
			size_t SrcSz = srcarr.m_SphereArr.size();
			size_t DestSz = destarr.m_SphereArr.size();
			for(size_t i = 0;i < SrcSz;i++){
				for(size_t j = 0;j < DestSz;j++){
					if(SPHERE_SPHERE(srcarr.m_SphereArr[i],destarr.m_SphereArr[j])){
						SrcHitIndex = i;
						DestHitIndex = j;
						return true;
					}
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_��AABB�Ƃ̍ŋߐړ_�𓾂�
		@param[in]	p	�_
		@param[in]	b	AABB
		@param[out]	retvec	�ŋߐړ_����������Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointAABB(const Vector3& p, const AABB& b,
			Vector3& retvec){
			for(int i = 0;i < 3;i++){
				float v = p[i];
				if(v < b.m_Min[i]){
					v = b.m_Min[i];
				}
				if(v > b.m_Max[i]){
					v = b.m_Max[i];
				}
				retvec[i] = v;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�_��AABB�Ƃ̋����̕����𑪂�
		@param[in]	p	�_
		@param[in]	b	AABB
		@return	�_��AABB�Ƃ̋����̕���
		*/
		//--------------------------------------------------------------------------------------
		static float SqDistPointAABB(const Vector3& p, const AABB& b){
			float sqDist = 0.0f;
			for(int i = 0;i < 3;i++){
				float v = p[i];
				if(v < b.m_Min[i]){
					sqDist += (b.m_Min[i] - v) * (b.m_Min[i] - v);
				}
				if(v > b.m_Max[i]){
					sqDist += (v - b.m_Max[i]) * (v - b.m_Max[i]);
				}
			}
			return sqDist;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����AABB�Ƃ̏Փ˔���
		@param[in]	sp	��
		@param[in]	b	AABB
		@param[out]	retvec	�ŋߐړ_����������Q��
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_AABB(const SPHERE& sp, const AABB& b,
			Vector3& retvec){
				//�ŋߐړ_�𓾂�
				ClosestPtPointAABB(sp.m_Center,b,retvec);
				//�����̕��������̔��a�̕������Ȃ�true
				if(SqDistPointAABB(sp.m_Center,b) <= (sp.m_Radius * sp.m_Radius)){
					return true;
				}
				return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABB��AABB�Ƃ̏Փ˔���
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool AABB_AABB(const AABB& a, const AABB& b){
			if(a.m_Max.x < b.m_Min.x || a.m_Min.x > b.m_Max.x)
				return false;
			if(a.m_Max.y < b.m_Min.y || a.m_Min.y > b.m_Max.y)
				return false;
			if(a.m_Max.z < b.m_Min.z || a.m_Min.z > b.m_Max.z)
				return false;
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABB a��AABB ���Ɋ��S�Ɋ܂܂�邩����
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	���S�Ɋ܂܂���true
		*/
		//--------------------------------------------------------------------------------------
		static bool AABB_IN_AABB(const AABB& a, const AABB& b){
			if(a.m_Min.x < b.m_Min.x || a.m_Max.x > b.m_Max.x)
				return false;
			if(a.m_Min.y < b.m_Min.y || a.m_Max.y > b.m_Max.y)
				return false;
			if(a.m_Min.z < b.m_Min.z || a.m_Max.z > b.m_Max.z)
				return false;
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����OBB�Ƃ̏Փ˔���
		@param[in]	sp	��
		@param[in]	obb	OBB
		@param[out]	retvec	�ŋߐړ_����������Q��
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_OBB(const SPHERE& sp, const OBB& obb, Vector3& retvec){
			ClosestPtPointOBB(sp.m_Center,obb,retvec);
			Vector3 v = retvec - sp.m_Center;
			return Vector3EX::Dot(v,v) <= sp.m_Radius * sp.m_Radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB��OBB�Ƃ̏Փ˔���
		@param[in]	obb1	OBB1
		@param[in]	obb2	OBB2
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_OBB(const OBB& obb1, const OBB& obb2){
			const float EPSILON = 1.175494e-37f;
			float R[3][3], AbsR[3][3];
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					R[i][j] = Vector3EX::Dot(obb1.m_Rot[i], obb2.m_Rot[j]);
					AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
				}
			}
			Vector3 t = obb2.m_Center - obb1.m_Center;
			t = Vector3(Vector3EX::Dot(t, obb1.m_Rot[0]),
							Vector3EX::Dot(t, obb1.m_Rot[1]),
							Vector3EX::Dot(t, obb1.m_Rot[2]));
			//��L=A0, L=A1, L=A2����
			float ra, rb;
			for(int i = 0; i < 3; i++){
				ra = obb1.m_Size[i];
				rb = obb2.m_Size[0] * AbsR[i][0] + obb2.m_Size[1] * AbsR[i][1] + obb2.m_Size[2] * AbsR[i][2];
				if(fabsf(t[i]) > ra + rb){
					return false;
				}
			}
			//��L=B0, L=B1, L=B2����
			for(int i = 0; i < 3; i++){
				ra = obb1.m_Size[0] * AbsR[0][i] + obb1.m_Size[1] * AbsR[1][i] + obb1.m_Size[2] * AbsR[2][i];
				rb = obb2.m_Size[i];
				if(fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb){
					return false;
				}
			}
			//��L=A0 X B0����
			ra = obb1.m_Size[1] * AbsR[2][0] + obb1.m_Size[2] * AbsR[1][0];
			rb = obb2.m_Size[1] * AbsR[0][2] + obb2.m_Size[2] * AbsR[0][1];
			if(fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb){
				return false;
			}
			//��L=A0 X B1����
			ra = obb1.m_Size[1] * AbsR[2][1] + obb1.m_Size[2] * AbsR[1][1];
			rb = obb2.m_Size[0] * AbsR[0][2] + obb2.m_Size[2] * AbsR[0][0];
			if(fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb){
				return false;
			}
			//��L=A0 X B2����
			ra = obb1.m_Size[1] * AbsR[2][2] + obb1.m_Size[2] * AbsR[1][2];
			rb = obb2.m_Size[0] * AbsR[0][1] + obb2.m_Size[1] * AbsR[0][0];
			if(fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb){
				return false;
			}
			//��L=A1 X B0����
			ra = obb1.m_Size[0] * AbsR[2][0] + obb1.m_Size[2] * AbsR[0][0];
			rb = obb2.m_Size[1] * AbsR[1][2] + obb2.m_Size[2] * AbsR[1][1];
			if(fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb){
				return false;
			}
			//��L=A1 X B1����
			ra = obb1.m_Size[0] * AbsR[2][1] + obb1.m_Size[2] * AbsR[0][1];
			rb = obb2.m_Size[0] * AbsR[1][2] + obb2.m_Size[2] * AbsR[1][0];
			if(fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb){
				return false;
			}
			//��L=A1 X B2����
			ra = obb1.m_Size[0] * AbsR[2][2] + obb1.m_Size[2] * AbsR[0][2];
			rb = obb2.m_Size[0] * AbsR[1][1] + obb2.m_Size[1] * AbsR[1][0];
			if(fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb){
				return false;
			}
			//��L=A2 X B0����
			ra = obb1.m_Size[0] * AbsR[1][0] + obb1.m_Size[1] * AbsR[0][0];
			rb = obb2.m_Size[1] * AbsR[2][2] + obb2.m_Size[2] * AbsR[2][1];
			if(fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb){
				return false;
			}
			//��L=A2 X B1����
			ra = obb1.m_Size[0] * AbsR[1][1] + obb1.m_Size[1] * AbsR[0][1];
			rb = obb2.m_Size[0] * AbsR[2][2] + obb2.m_Size[2] * AbsR[2][0];
			if(fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb){
				return false;
			}
			//��L=A2 X B2����
			ra = obb1.m_Size[0] * AbsR[1][2] + obb1.m_Size[1] * AbsR[0][2];
			rb = obb2.m_Size[0] * AbsR[2][1] + obb2.m_Size[1] * AbsR[2][0];
			if(fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb){
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	����ab�Ɩʔ���
		@param[in]	a	�����J�n�_
		@param[in]	b	�����I���_
		@param[in]	p	��
		@param[out]	t	�ŋߐړ_�̐�����̈ʒu�̊����̖߂�l
		@param[out]	q	�ŋߐړ_�̖߂�l
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_PLANE(const Vector3& a, const Vector3& b,
			const PLANE& p,float& t,Vector3& q){
			Vector3 ab = b - a;
			t = (p.m_DotValue - Vector3EX::Dot(p.m_Normal,a)) / Vector3EX::Dot(p.m_Normal,ab);
			if(t >= 0.0f && t <= 1.0f){
				q = a + (ab * t);
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������AABB�̃��C����
		@param[in]	p0	�����J�n�_
		@param[in]	p1	�����I���_
		@param[in]	b	AABB
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_AABB(const Vector3& p0, const Vector3& p1, const AABB& b){
			const float EPSILON = 1.175494e-37f;
			Vector3 c = (b.m_Min + b.m_Max) * 0.5f;
			Vector3 e = b.m_Max - c;
			Vector3 m = (p0 + p1) * 0.5f;
			Vector3 d = p1 - m;
			m = m - c;
			float adx = fabsf(d.x);
			if(fabsf(m.x) > e.x + adx){
				return false;
			}
			float ady = fabsf(d.y);
			if(fabsf(m.y) > e.y + ady){
				return false;
			}
			float adz = fabsf(d.z);
			if(fabsf(m.z) > e.z + adz){
				return false;
			}
			adx += EPSILON;
			ady += EPSILON;
			adz += EPSILON;
			if(fabsf(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady){
				return false;
			}
			if(fabsf(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx){
				return false;
			}
			if(fabsf(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx){
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������OBB�̃��C����
		@param[in]	p0	�����J�n�_
		@param[in]	p1	�����I���_
		@param[in]	obb	OBB
		@return	�������Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_OBB(const Vector3& p0, const Vector3& p1, const OBB& obb){
			const float EPSILON = 1.175494e-37f;

			Vector3 m = (p0 + p1) * 0.5f;
			Vector3 d = p1 - m;
			m = m - obb.m_Center;
			m = Vector3(Vector3EX::Dot(obb.m_Rot[0], m), Vector3EX::Dot(obb.m_Rot[1], m), Vector3EX::Dot(obb.m_Rot[2], m));
			d = Vector3(Vector3EX::Dot(obb.m_Rot[0], d), Vector3EX::Dot(obb.m_Rot[1], d), Vector3EX::Dot(obb.m_Rot[2], d));

			float adx = fabsf(d.x);
			if(fabsf(m.x) > obb.m_Size.x + adx) return false;
			float ady = fabsf(d.y);
			if(fabsf(m.y) > obb.m_Size.y + ady) return false;
			float adz = fabsf(d.z);
			if(fabsf(m.z) > obb.m_Size.z + adz) return false;
			adx += EPSILON; 
			ady += EPSILON; 
			adz += EPSILON;
        
			if(fabsf(m.y * d.z - m.z * d.y) > obb.m_Size.y * adz + obb.m_Size.z * ady ) return false;
			if(fabsf(m.z * d.x - m.x * d.z) > obb.m_Size.x * adz + obb.m_Size.z * adx ) return false;
			if(fabsf(m.x * d.y - m.y * d.x) > obb.m_Size.x * ady + obb.m_Size.y * adx ) return false;

			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Sphere�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestSp	Dest��
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereSphere(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const SPHERE& DestSp, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
			if (!HitTest::SPHERE_SPHERE(SrcSp2, DestSp)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereSphere(SrcSp, SrcVelocity, DestSp, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereSphere(SrcSp, SrcVelocity, DestSp, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Capsule�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestCap	Dest�J�v�Z��
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereCapsule(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const CAPSULE& DestCap,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
			Vector3 RetVec;
			if (!HitTest::SPHERE_CAPSULE(SrcSp2, DestCap, RetVec)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereCapsule(SrcSp, SrcVelocity, DestCap, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereCapsule(SrcSp, SrcVelocity, DestCap, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Capsule�Ɠ����Ȃ�Capsule�̏Փ˔���
		@param[in]	SrcCap	Src�̃J�v�Z��
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestCap	Dest�J�v�Z��
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleCapsule(const CAPSULE& SrcCapsule, const Vector3& SrcVelocity,
			const CAPSULE& DestCap,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;

			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;

			//���S�����_�̌��̃J�v�Z�����쐬
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(Vector3(0, 0, 0));
			//���_�J�v�Z���ŃX�P�[�����O
			//�X�P�[�����O�s��̍쐬
			Matrix4X4 ScalMat;
			ScalMat.Scaling(Scale, Scale, Scale);
			//�e���_���X�P�[�����O
			SrcCapsule2.m_PointA = Vector3EX::Transform(SrcBaseCapsule.m_PointA, ScalMat);
			SrcCapsule2.m_PointB = Vector3EX::Transform(SrcBaseCapsule.m_PointB, ScalMat);
			//���S���ړ�
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);

			Vector3 RetVec1, RetVec2;
			if (!HitTest::CAPSULE_CAPSULE(SrcCapsule2, DestCap, RetVec1, RetVec2)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleCapsule(SrcCapsule, SrcVelocity, DestCap, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestCapsuleCapsule(SrcCapsule, SrcVelocity, DestCap, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Aabb�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestAabb	Dest��AABB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereAabb(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const AABB& DestAabb,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
			Vector3 RetVec;
			if (!HitTest::SPHERE_AABB(SrcSp2, DestAabb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereAabb(SrcSp, SrcVelocity, DestAabb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereAabb(SrcSp, SrcVelocity, DestAabb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphere�Ɠ����Ȃ�Obb�̏Փ˔���
		@param[in]	SrcSp	Src�̋�
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereObb(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const OBB& DestObb,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
			Vector3 RetVec;
			if (!HitTest::SPHERE_OBB(SrcSp2, DestObb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereObb(SrcSp, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereObb(SrcSp, SrcVelocity, DestObb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z����Obb�̍ŋߐړ_�𓾂�
		@param[in]	cp	�J�v�Z��
		@param[in]	obb	OBB
		@param[out]	flg	2���̂Ƃ̈ʒu�֌W
		@return	�ŋߐړ_
		*/
		//--------------------------------------------------------------------------------------
		static Vector3 ClosestPtCapsuleOBB(const CAPSULE& cp, const OBB& obb, int& flg){
			SPHERE Sp;
			Sp.m_Center = cp.m_PointA;
			Sp.m_Radius = cp.m_Radius;
			Vector3 retvec;
			//�X�^�[�g�ʒu�ōŋߐړ_�𓾂�
			HitTest::SPHERE_OBB(Sp, obb, retvec);
			//���ς�}��
			Vector3 Base = cp.m_PointB - cp.m_PointA;
			Base.Normalize();
			Vector3 Dest = retvec - cp.m_PointA;
			float dot = Base.Dot(Dest);
			if (dot < 0){
				//�X�^�[�g�ʒu�̋��̂̊O��
				//retvec�͗L��
				flg = -1;
				return retvec;
			}
			float  size = Vector3EX::Length(cp.m_PointB - cp.m_PointA);
			if (dot > size){
				//�I�_����ɂ���
				Sp.m_Center = cp.m_PointB;
				HitTest::SPHERE_OBB(Sp, obb, retvec);
				//�I�_�ōŋߐړ_���Ƃ�
				flg = 1;
				return retvec;
			}
			//���S��obb�̍ŋߐړ_�𓾂�
			HitTest::ClosestPtPointOBB(cp.GetCenter(), obb, retvec);
			float t;
			Vector3 SegPoint;
			HitTest::ClosetPtPointSegment(retvec, cp.m_PointA, cp.m_PointB, t, SegPoint);
			Vector3 Span = retvec - SegPoint;
			Span.Normalize();
			Span *= cp.m_Radius;
			SegPoint += Span;
			retvec = SegPoint;
			flg = 0;
			return retvec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z����Obb�̏Փ˔���
		@param[in]	cp	�J�v�Z��
		@param[in]	obb	OBB
		@param[out]	retvec	�ŋߐړ_
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_OBB(const CAPSULE& cp, const OBB& obb, Vector3& retvec){
			//�X�B�[�v�����鋅
			SPHERE StartSp, EndSp;
			//�e�_��obb�̍ŋߐړ_�𓾂�
			Vector3 ToObb;
			HitTest::ClosestPtPointOBB(cp.m_PointA, obb, ToObb);
			float LenA = Vector3EX::Length(cp.m_PointA - ToObb);
			HitTest::ClosestPtPointOBB(cp.m_PointB, obb, ToObb);
			float LenB = Vector3EX::Length(cp.m_PointB - ToObb);
			if (LenA < LenB){
				//�X�^�[�g��A��
				StartSp.m_Center = cp.m_PointA;
				StartSp.m_Radius = cp.m_Radius;
				EndSp.m_Center = cp.m_PointB;
				EndSp.m_Radius = cp.m_Radius;
			}
			else{
				//�X�^�[�g��B��
				StartSp.m_Center = cp.m_PointB;
				StartSp.m_Radius = cp.m_Radius;
				EndSp.m_Center = cp.m_PointA;
				EndSp.m_Radius = cp.m_Radius;
			}
			//�J�v�Z����OBB�̍ŋߐړ_�𓾂�i�Փ˂��Ă邩�ǂ����͊֌W�Ȃ��j
			int flg;
			retvec = ClosestPtCapsuleOBB(cp, obb,flg);
			float HitTime;
			Vector3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereObb(StartSp, Velocity,obb, 0, 1.0f, HitTime)){
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�v�Z���Ɠ����Ȃ�Obb�̏Փ˔���
		@param[in]	SrcCapsule	Src�̃J�v�Z��
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleObb(const CAPSULE& SrcCapsule, const Vector3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//���S�����_�̌��̃J�v�Z�����쐬
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(Vector3(0, 0, 0));
			//���_�J�v�Z���ŃX�P�[�����O
			//�X�P�[�����O�s��̍쐬
			Matrix4X4 ScalMat;
			ScalMat.Scaling(Scale, Scale, Scale);
			//�e���_���X�P�[�����O
			SrcCapsule2.m_PointA = Vector3EX::Transform(SrcBaseCapsule.m_PointA, ScalMat);
			SrcCapsule2.m_PointB = Vector3EX::Transform(SrcBaseCapsule.m_PointB, ScalMat);
			//���S���ړ�
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			Vector3 RetVec;
			if (!HitTest::CAPSULE_OBB(SrcCapsule2, DestObb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleObb(SrcCapsule, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestCapsuleObb(SrcCapsule, SrcVelocity, DestObb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB�Ɠ����Ȃ�OBB�̏Փ˔���i�T�u�֐��Ƃ��Ď����j
		@param[in]	SrcObb	Src��Obb
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbObbSub(const OBB& SrcObb, const Vector3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			OBB SrcObb2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcObb2.m_Center = SrcObb.m_Center + SrcVelocity * mid;
			//OBB�̊e�ӂ̒������g��
			SrcObb2.m_Size.x = (mid - StartTime) * SrcVelocity.Length() + SrcObb.m_Size.x;
			SrcObb2.m_Size.y = (mid - StartTime) * SrcVelocity.Length() + SrcObb.m_Size.y;
			SrcObb2.m_Size.z = (mid - StartTime) * SrcVelocity.Length() + SrcObb.m_Size.z;
			if (!HitTest::OBB_OBB(SrcObb2, DestObb)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestObbObbSub(SrcObb, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestObbObbSub(SrcObb, SrcVelocity, DestObb, mid, EndTime, HitTime);

		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBB�Ɠ����Ȃ�OBB�̏Փ˔���iSrcObb��SPHERE���ł���OBB�j
		@param[in]	SrcObb	Src��Obb
		@param[in]	SrcVelocity	�\�[�X���x
		@param[in]	DestObb	Dest��OBB
		@param[in]	StartTime	�J�n����
		@param[in]	EndTime	�I������
		@param[out]	HitTime	�q�b�g����
		@return	�Փ˂��Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbObb(const OBB& SrcObb, const Vector3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE BeforeSrcSphere;
			BeforeSrcSphere.m_Center = SrcObb.m_Center;
			BeforeSrcSphere.m_Radius = SrcObb.m_Size.Length();
			if (!CollisionTestSphereObb(BeforeSrcSphere, SrcVelocity, DestObb, StartTime, EndTime, HitTime)){
				//�Փ˂��Ȃ����OBB���Փ˂Ȃ�
				return false;
			}
			return CollisionTestObbObbSub(SrcObb, SrcVelocity,DestObb, StartTime, EndTime,HitTime);
		}
	};


}
//end of namespace basecross.

