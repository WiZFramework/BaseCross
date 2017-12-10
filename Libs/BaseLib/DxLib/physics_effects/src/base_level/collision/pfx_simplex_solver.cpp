/*
Physics Effects Copyright(C) 2012 Sony Computer Entertainment Inc.
All rights reserved.

Physics Effects is open software; you can redistribute it and/or
modify it under the terms of the BSD License.

Physics Effects is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the BSD License for more details.

A copy of the BSD License is distributed with
Physics Effects under the filename: physics_effects_license.txt
*/
#include "stdafx.h"


#include "pfx_simplex_solver.h"
#include "pfx_gjk_solver.h"

namespace sce {
namespace PhysicsEffects {

bool PfxSimplexSolver::closest(PfxVector3& v)
{
	bool ret = false;

	bc.reset();

	switch(numVertices) {
		case 0:
		ret = false;
		break;

		case 1:
		{
			PfxVector3 tmpP = P[0];
			PfxVector3 tmpQ = Q[0];
			v = tmpP-tmpQ;
			bc.reset();
			bc.setBarycentricCoordinates(1.0f,0.0f,0.0f,0.0f);
			ret = bc.isValid();
		}
		break;

		case 2:
		{
			PfxVector3 dir = W[1] - W[0];
			PfxFloat t = dot(-W[0],dir) / dot(dir,dir);

			if(t < 0.0f) t = 0.0f;
			if(t > 1.0f) t = 1.0f;

			bc.setBarycentricCoordinates(1-t,t,0.0f,0.0f);

			PfxVector3 tmpP = P[0] + t * (P[1] - P[0]);
			PfxVector3 tmpQ = Q[0] + t * (Q[1] - Q[0]);
			v = tmpP - tmpQ;

			reduceVertices();

			ret = bc.isValid();
		}
		break;

		case 3: 
		{ 
			const PfxVector3& a = W[0]; 
			const PfxVector3& b = W[1]; 
			const PfxVector3& c = W[2]; 
			
			closestPointTriangleFromOrigin(a,b,c,bc);
			
		PfxVector3 tmpP = P[0] * bc.barycentricCoords[0] + P[1] * bc.barycentricCoords[1] + P[2] * bc.barycentricCoords[2]; 
		PfxVector3 tmpQ = Q[0] * bc.barycentricCoords[0] + Q[1] * bc.barycentricCoords[1] + Q[2] * bc.barycentricCoords[2]; 
			v = tmpP - tmpQ;
			reduceVertices(); 
			ret = bc.isValid(); 
		}
		break; 

		case 4:
		{
			const PfxVector3& a = W[0];
			const PfxVector3& b = W[1];
			const PfxVector3& c = W[2];
			const PfxVector3& d = W[3];
			
			if(!closestPointTetrahedronFromOrigin(a,b,c,d,bc)) {
				// origin is in the tetrahedron
				ret = true;
				v = PfxVector3(0.0f);
				break;
			}
			
		PfxVector3 tmpP = P[0] * bc.barycentricCoords[0] + P[1] * bc.barycentricCoords[1] + P[2] * bc.barycentricCoords[2] + P[3] * bc.barycentricCoords[3];
		PfxVector3 tmpQ = Q[0] * bc.barycentricCoords[0] + Q[1] * bc.barycentricCoords[1] + Q[2] * bc.barycentricCoords[2] + Q[3] * bc.barycentricCoords[3];
			v = tmpP - tmpQ;
			reduceVertices();
			ret = bc.isValid();
		}
		break;
	};

	return ret;
}

bool PfxSimplexSolver::closestPointTriangleFromOrigin(const PfxVector3 &a,const  PfxVector3 &b,const  PfxVector3 &c,PfxBarycentricCoords& result)
{
	result.usedVertices = 0;
	PfxVector3 p(0.0f);

	PfxVector3 ab = b - a;
	PfxVector3 ac = c - a;
	PfxVector3 ap = p - a;
	PfxFloat d1 = dot(ab,ap);
	PfxFloat d2 = dot(ac,ap);
	if(d1 <= 0.0f && d2 <= 0.0f) {
		result.closest = a;
		result.setBarycentricCoordinates(1.0f,0.0f,0.0f,0.0f);
		return true;
	}

	PfxVector3 bp = p - b;
	PfxFloat d3 = dot(ab,bp);
	PfxFloat d4 = dot(ac,bp);
	if(d3 >= 0.0f && d4 <= d3) {
		result.closest = b;
		result.setBarycentricCoordinates(0.0f,1.0f,0.0f,0.0f);
		return true;
	}

	PfxFloat vc = d1*d4 - d3*d2;
	if(vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		PfxFloat v = d1 / (d1 - d3);
		result.closest = a + v * ab;
		result.setBarycentricCoordinates(1.0f-v,v,0.0f,0.0f);
		return true;
	}

	PfxVector3 cp = p - c;
	PfxFloat d5 = dot(ab,cp);
	PfxFloat d6 = dot(ac,cp);
	if(d6 >= 0.0f && d5 <= d6) {
		result.closest = c;
		result.setBarycentricCoordinates(0.0f,0.0f,1.0f,0.0f);
		return true;
	}

	PfxFloat vb = d5*d2 - d1*d6;
	if(vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		PfxFloat w = d2 / (d2 - d6);
		result.closest = a + w * ac;
		result.setBarycentricCoordinates(1.0f-w,0.0f,w,0.0f);
		return true;
	}

	PfxFloat va = d3*d6 - d5*d4;
	if(va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		PfxFloat w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		result.closest = b + w * (c - b);
		result.setBarycentricCoordinates(0.0f,1.0f-w,w,0.0f);
		return true;		
	}

PfxFloat denom = 1.0f / (va + vb + vc);
	PfxFloat u = va * denom;
	PfxFloat v = vb * denom;
	PfxFloat w = vc * denom;
	
	result.closest = u * a + v * b + w * c;
	result.setBarycentricCoordinates(u,v,w,0.0f);
	
	return true;
}

#if 0
bool PfxSimplexSolver::closestPointTetrahedronFromOrigin(const PfxVector3 &a,const  PfxVector3 &b,const  PfxVector3 &c,const  PfxVector3 &d,PfxBarycentricCoords& finalResult)
{
	PfxBarycentricCoords tempResult;
	PfxVector3 p(0.0f);

	finalResult.closest = p;
	finalResult.usedVertices = 0;

	bool pointOutsideABC = originOutsideOfPlane(a, b, c, d);
	bool pointOutsideACD = originOutsideOfPlane(a, c, d, b);
	bool pointOutsideADB = originOutsideOfPlane(a, d, b, c);
	bool pointOutsideBDC = originOutsideOfPlane(b, d, c, a);

	if(!pointOutsideABC && !pointOutsideACD && !pointOutsideADB && !pointOutsideBDC)
		return false;

	PfxFloat bestSqDist = SCE_PFX_FLT_MAX;

	if(pointOutsideABC) {
		closestPointTriangleFromOrigin(a, b, c,tempResult);
		PfxVector3 q = tempResult.closest;
		PfxFloat sqDist = dot((q - p),(q - p));
		if(sqDist < bestSqDist) {
			bestSqDist = sqDist;
			finalResult.closest = q;
			finalResult.setBarycentricCoordinates(
					tempResult.barycentricCoords[0],
					tempResult.barycentricCoords[1],
					tempResult.barycentricCoords[2],
					0);
		}
	}
  
	if(pointOutsideACD) {
		closestPointTriangleFromOrigin(a, c, d,tempResult);
		PfxVector3 q = tempResult.closest;
		PfxFloat sqDist = dot((q - p),(q - p));
		if(sqDist < bestSqDist) {
			bestSqDist = sqDist;
			finalResult.closest = q;
			finalResult.setBarycentricCoordinates(
					tempResult.barycentricCoords[0],
					0,
					tempResult.barycentricCoords[1],
					tempResult.barycentricCoords[2]);
		}
	}
	
	if(pointOutsideADB) {
		closestPointTriangleFromOrigin(a, d, b,tempResult);
		PfxVector3 q = tempResult.closest;
		PfxFloat sqDist = dot((q - p),(q - p));
		if(sqDist < bestSqDist) {
			bestSqDist = sqDist;
			finalResult.closest = q;
			finalResult.setBarycentricCoordinates(
					tempResult.barycentricCoords[0],
					tempResult.barycentricCoords[2],
					0,
					tempResult.barycentricCoords[1]);
		}
	}

	if(pointOutsideBDC) {
		closestPointTriangleFromOrigin(b, d, c,tempResult);
		PfxVector3 q = tempResult.closest;
		PfxFloat sqDist = dot((q - p),(q - p));
		if(sqDist < bestSqDist) {
			bestSqDist = sqDist;
			finalResult.closest = q;
			finalResult.setBarycentricCoordinates(
					0,
					tempResult.barycentricCoords[0],
					tempResult.barycentricCoords[2],
					tempResult.barycentricCoords[1]);
		}
	}

	return true;
}
#else
bool PfxSimplexSolver::closestPointTetrahedronFromOrigin(const PfxVector3 &a,const  PfxVector3 &b,const  PfxVector3 &c,const  PfxVector3 &d,PfxBarycentricCoords& finalResult)
{
	PfxBarycentricCoords tempResult;
	PfxVector3 p(0.0f);

	finalResult.closest = p;
	finalResult.usedVertices = 0;

	PfxVector3 ap = p - a;
	PfxVector3 ac = c - a;
	PfxVector3 ab = b - a;
	PfxVector3 ad = d - a;
	PfxVector3 bc = c - b;
	PfxVector3 bd = d - b;

	PfxVector3 nabc = cross(ab,ac);
	PfxVector3 nacd = cross(ac,ad);
	PfxVector3 nadb = cross(ad,ab);
	PfxVector3 nbdc = cross(bd,bc);

	bool pointOutsideABC = (dot(-a,nabc) * dot(ad,nabc)) < 0.0f;
	bool pointOutsideACD = (dot(-a,nacd) * dot(ab,nacd)) < 0.0f;
	bool pointOutsideADB = (dot(-a,nadb) * dot(ac,nadb)) < 0.0f;
	bool pointOutsideBDC = (dot( b,nbdc) * dot(ab,nbdc)) < 0.0f;

	if(!pointOutsideABC && !pointOutsideACD && !pointOutsideADB && !pointOutsideBDC)
		return false;

	//////////////////////////////////////////////////////////////////////////////
	// Voronoi check (Vertex)

	// A
	PfxFloat d1 = dot(ap,ac);
	PfxFloat d2 = dot(ap,ab);
	PfxFloat d3 = dot(ap,ad);
	if(d1 <= 0.0f && d2 <= 0.0f && d3 <= 0.0f) {
		finalResult.closest = a;
		finalResult.setBarycentricCoordinates(1.0f,0.0f,0.0f,0.0f);
		return true;
	}

	PfxVector3 bp = p - b;

	PfxFloat d4 = dot(bp,bc);
	PfxFloat d5 = dot(bp,-ab);
	PfxFloat d6 = dot(bp,bd);

	// B
	if(d4 <= 0.0f && d5 <= 0.0f && d6 <= 0.0f) {
		finalResult.closest = b;
		finalResult.setBarycentricCoordinates(0.0f,1.0f,0.0f,0.0f);
		return true;
	}

	PfxVector3 cp = p - c;
	PfxVector3 cd = d - c;

	PfxFloat d7 = dot(cp,-ac);
	PfxFloat d8 = dot(cp,-bc);
	PfxFloat d9 = dot(cp,cd);

	// C
	if(d7 <= 0.0f && d8 <= 0.0f && d9 <= 0.0f) {
		finalResult.closest = c;
		finalResult.setBarycentricCoordinates(0.0f,0.0f,1.0f,0.0f);
		return true;
	}

	PfxVector3 dp = p - d;

	PfxFloat d10 = dot(dp,-ad);
	PfxFloat d11 = dot(dp,-bd);
	PfxFloat d12 = dot(dp,-cd);
	
	// D
	if(d10 <= 0.0f && d11 <= 0.0f && d12 <= 0.0f) {
		finalResult.closest = d;
		finalResult.setBarycentricCoordinates(0.0f,0.0f,0.0f,1.0f);
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// Voronoi check (Edge)
	
	// AB
#if 0
	PfxVector3 pa_x_pb = cross(ap,bp);
	PfxFloat vabc_ab = dot(nabc,pa_x_pb);
	PfxFloat vadb_ab = dot(nadb,-pa_x_pb);
#else
	PfxFloat vabc_ab = d5*d1-d2*d5+d2*d4;
	PfxFloat vadb_ab = d2*d6-d2*d5+d3*d5;
#endif
	if(d2 >= 0.0f && d5 >= 0.0f && vabc_ab <= 0.0f && vadb_ab <= 0.0f) {
		PfxFloat v = dot(ap,ab) / dot(ab,ab);
		finalResult.closest = a + v * ab;
		finalResult.setBarycentricCoordinates(1.0f-v,v,0.0f,0.0f);
		return true;
	}

	// AC
#if 0
	PfxVector3 pc_x_pa = cross(cp,ap);
	PfxFloat vabc_ac = dot(nabc,pc_x_pa);
	PfxFloat vacd_ac = dot(nacd,-pc_x_pa);
#else
	PfxFloat vabc_ac = d1*d8-d1*d7+d2*d7;
	PfxFloat vacd_ac = d1*d9-d1*d7+d7*d3;
#endif
	if(d1 >= 0.0f && d7 >= 0.0f && vabc_ac <= 0.0f && vacd_ac <= 0.0f) {
		PfxFloat v = dot(ap,ac) / dot(ac,ac);
		finalResult.closest = a + v * ac;
		finalResult.setBarycentricCoordinates(1.0f-v,0.0f,v,0.0f);
		return true;
	}
	
	// AD
#if 0
	PfxVector3 pd_x_pa = cross(dp,ap);
	PfxFloat vacd_ad = dot(nacd,pd_x_pa);
	PfxFloat vadb_ad = dot(nadb,-pd_x_pa);
#else
	PfxFloat vacd_ad = d3*d12-d3*d10+d1*d10;
	PfxFloat vadb_ad = d3*d11-d3*d10+d2*d10;
#endif
	if(d3 >= 0.0f && d10 >= 0.0f && vacd_ad <= 0.0f && vadb_ad <= 0.0f) {	
		PfxFloat v = dot(ap,ad) / dot(ad,ad);
		finalResult.closest = a + v * ad;
		finalResult.setBarycentricCoordinates(1.0f-v,0.0f,0.0f,v);
		return true;
	}
	
	// BD
#if 0
	PfxVector3 pd_x_pb = cross(dp,bp);
	PfxFloat vadb_bd = dot(nadb,pd_x_pb);
	PfxFloat vbdc_bd = dot(nbdc,-pd_x_pb);
#else
	PfxFloat vadb_bd = d5*d11+d6*d10-d6*d11;
	PfxFloat vbdc_bd = d6*d12-d6*d11+d4*d11;
#endif
	if(d6 >= 0.0f && d11 >= 0.0f && vadb_bd <= 0.0f && vbdc_bd <= 0.0f) {
		PfxFloat v = dot(bp,bd) / dot(bd,bd);
		finalResult.closest = b + v * bd;
		finalResult.setBarycentricCoordinates(0.0f,1.0f-v,0.0f,v);
		return true;
	}

	// BC
#if 0
	PfxVector3 pb_x_pc = cross(bp,cp);
	PfxFloat vabc_bc = dot(nabc,pb_x_pc);
	PfxFloat vbdc_bc = dot(nbdc,-pb_x_pc);
#else
	PfxFloat vabc_bc = d7*d4+d8*d5-d8*d4;
	PfxFloat vbdc_bc = d4*d9-d4*d8+d6*d8;
#endif
	if(d4 >= 0.0f && d8 >= 0.0f && vabc_bc <= 0.0f && vbdc_bc <= 0.0f) {
		PfxFloat v = dot(bp,bc) / dot(bc,bc);
		finalResult.closest = b + v * bc;
		finalResult.setBarycentricCoordinates(0.0f,1.0f-v,v,0.0f);
		return true;
	}
	
	// CD
#if 0
	PfxVector3 pc_x_pd = cross(cp,dp);
	PfxFloat vacd_cd = dot(nacd,pc_x_pd);
	PfxFloat vbdc_cd = dot(nbdc,-pc_x_pd);
#else
	PfxFloat vacd_cd = d10*d9+d12*d7-d12*d9;
	PfxFloat vbdc_cd = d8*d12+d9*d11-d9*d12;
#endif
	if(d9 >= 0.0f && d12 >= 0.0f && vacd_cd <= 0.0f && vbdc_cd <= 0.0f) {
		PfxFloat v = dot(cp,cd) / dot(cd,cd);
		finalResult.closest = c + v * cd;
		finalResult.setBarycentricCoordinates(0.0f,0.0f,1.0f-v,v);
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// Å‹ßÚ“_‚Í–ÊABC,ACD,ADB,BDCã‚É‘¶Ý
	
	PfxFloat bestSqDist = SCE_PFX_FLT_MAX;
	
	// ABC
	if(pointOutsideABC) {
	PfxFloat denom = 1.0f / (vabc_ab + vabc_ac + vabc_bc);
		PfxFloat u = vabc_bc * denom;
		PfxFloat v = vabc_ac * denom;
		PfxFloat w = vabc_ab * denom;
		PfxVector3 q = u * a + v * b + w * c;
		PfxFloat sqDist = dot(q,q);
		if(sqDist < bestSqDist && u >= 0.0f && v >= 0.0f && w >= 0.0f) {
			bestSqDist = sqDist;
			finalResult.closest = q;
			finalResult.setBarycentricCoordinates(u,v,w,0.0f);
		}
	}
	
	// ACD
	if(pointOutsideACD) {
	PfxFloat denom = 1.0f / (vacd_ac + vacd_ad + vacd_cd);
		PfxFloat u = vacd_cd * denom;
		PfxFloat v = vacd_ad * denom;
		PfxFloat w = vacd_ac * denom;
		PfxVector3 q = u * a + v * c + w * d;
		PfxFloat sqDist = dot(q,q);
		if(sqDist < bestSqDist && u >= 0.0f && v >= 0.0f && w >= 0.0f) {
			bestSqDist = sqDist;
			finalResult.closest = q;
			finalResult.setBarycentricCoordinates(u,0.0f,v,w);
		}
	}
	
	// ADB
	if(pointOutsideADB) {
	PfxFloat denom = 1.0f / (vadb_ab + vadb_ad + vadb_bd);
		PfxFloat u = vadb_bd * denom;
		PfxFloat v = vadb_ab * denom;
		PfxFloat w = vadb_ad * denom;
		PfxVector3 q = u * a + v * d + w * b;
		PfxFloat sqDist = dot(q,q);
		if(sqDist < bestSqDist && u >= 0.0f && v >= 0.0f && w >= 0.0f) {
			bestSqDist = sqDist;
			finalResult.closest = q;
			finalResult.setBarycentricCoordinates(u,w,0.0f,v);
		}
	}
	
	// BDC
	if(pointOutsideBDC) {
	PfxFloat denom = 1.0f / (vbdc_bd + vbdc_bc + vbdc_cd);
		PfxFloat u = vbdc_cd * denom;
		PfxFloat v = vbdc_bc * denom;
		PfxFloat w = vbdc_bd * denom;
		PfxVector3 q = u * b + v * d + w * c;
		PfxFloat sqDist = dot(q,q);
		if(sqDist < bestSqDist && u >= 0.0f && v >= 0.0f && w >= 0.0f ) {
			bestSqDist = sqDist;
			finalResult.closest = q;
			finalResult.setBarycentricCoordinates(0.0f,u,w,v);
		}
	}
	
	return true;
}
#endif

} //namespace PhysicsEffects
} //namespace sce
