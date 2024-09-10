//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H
#pragma once

#include "vphysics_interface.h"
#include "ivu_types.hxx"
#include "utlvector.h"

class IVP_Environment;
class CSleepObjects;
class CPhysicsListenerCollision;
class CPhysicsListenerConstraint;
class IVP_Listener_Collision;
class IVP_Listener_Constraint;
class IVP_Listener_Object;
class IVP_Controller;
class CPhysicsFluidController;
class CCollisionSolver;
class CPhysicsObject;
class CDeleteQueue;
class IVPhysicsDebugOverlay;
struct constraint_limitedhingeparams_t;
struct vphysics_save_iphysicsobject_t;

class CPhysicsEnvironment : public IPhysicsEnvironment
{
public:
	CPhysicsEnvironment( void );
	~CPhysicsEnvironment( void );

	void SetDebugOverlay( CreateInterfaceFn debugOverlayFactory ) override;
	IVPhysicsDebugOverlay *GetDebugOverlay( void ) override;

	void			SetGravity( const Vector& gravityVector ) override;
	IPhysicsObject	*CreatePolyObject( const CPhysCollide *pCollisionModel, int materialIndex, const Vector& position, const QAngle& angles, objectparams_t *pParams ) override;
	IPhysicsObject	*CreatePolyObjectStatic( const CPhysCollide *pCollisionModel, int materialIndex, const Vector& position, const QAngle& angles, objectparams_t *pParams ) override;
	unsigned int	GetObjectSerializeSize( IPhysicsObject *pObject ) const override;
	void			SerializeObjectToBuffer( IPhysicsObject *pObject, unsigned char *pBuffer, unsigned int bufferSize ) override;
	IPhysicsObject *UnserializeObjectFromBuffer( void *pGameData, unsigned char *pBuffer, unsigned int bufferSize, bool enableCollisions ) override;
	


	IPhysicsSpring	*CreateSpring( IPhysicsObject *pObjectStart, IPhysicsObject *pObjectEnd, springparams_t *pParams ) override;
	IPhysicsFluidController	*CreateFluidController( IPhysicsObject *pFluidObject, fluidparams_t *pParams ) override;
	IPhysicsConstraint *CreateRagdollConstraint( IPhysicsObject *pReferenceObject, IPhysicsObject *pAttachedObject, IPhysicsConstraintGroup *pGroup, const constraint_ragdollparams_t &ragdoll ) override;

	IPhysicsConstraint *CreateHingeConstraint( IPhysicsObject *pReferenceObject, IPhysicsObject *pAttachedObject, IPhysicsConstraintGroup *pGroup, const constraint_hingeparams_t &hinge ) override;
	virtual IPhysicsConstraint *CreateLimitedHingeConstraint( IPhysicsObject *pReferenceObject, IPhysicsObject *pAttachedObject, IPhysicsConstraintGroup *pGroup, const constraint_limitedhingeparams_t &hinge );
	IPhysicsConstraint *CreateFixedConstraint( IPhysicsObject *pReferenceObject, IPhysicsObject *pAttachedObject, IPhysicsConstraintGroup *pGroup, const constraint_fixedparams_t &fixed ) override;
	IPhysicsConstraint *CreateSlidingConstraint( IPhysicsObject *pReferenceObject, IPhysicsObject *pAttachedObject, IPhysicsConstraintGroup *pGroup, const constraint_slidingparams_t &sliding ) override;
	IPhysicsConstraint *CreateBallsocketConstraint( IPhysicsObject *pReferenceObject, IPhysicsObject *pAttachedObject, IPhysicsConstraintGroup *pGroup, const constraint_ballsocketparams_t &ballsocket ) override;
	IPhysicsConstraint *CreatePulleyConstraint( IPhysicsObject *pReferenceObject, IPhysicsObject *pAttachedObject, IPhysicsConstraintGroup *pGroup, const constraint_pulleyparams_t &pulley ) override;
	IPhysicsConstraint *CreateLengthConstraint( IPhysicsObject *pReferenceObject, IPhysicsObject *pAttachedObject, IPhysicsConstraintGroup *pGroup, const constraint_lengthparams_t &length ) override;

	IPhysicsConstraintGroup *CreateConstraintGroup( const constraint_groupparams_t &group ) override;
	void DestroyConstraintGroup( IPhysicsConstraintGroup *pGroup ) override;

	void			Simulate( float deltaTime ) override;
	float			GetSimulationTimestep() const override;
	void			SetSimulationTimestep( float timestep ) override;
	float			GetSimulationTime() const override;
	float			GetNextFrameTime() const override;
	bool			IsInSimulation() const override;

	void DestroyObject( IPhysicsObject * ) override;
	void DestroySpring( IPhysicsSpring * ) override;
	void DestroyFluidController( IPhysicsFluidController * ) override;
	void DestroyConstraint( IPhysicsConstraint * ) override;

	void SetCollisionEventHandler( IPhysicsCollisionEvent *pCollisionEvents ) override;
	void SetObjectEventHandler( IPhysicsObjectEvent *pObjectEvents ) override;
	void SetConstraintEventHandler( IPhysicsConstraintEvent *pConstraintEvents ) override;

	IPhysicsShadowController *CreateShadowController( IPhysicsObject *pObject, bool allowTranslation, bool allowRotation ) override;
	void DestroyShadowController( IPhysicsShadowController * ) override;
	IPhysicsMotionController *CreateMotionController( IMotionEvent *pHandler ) override;
	void DestroyMotionController( IPhysicsMotionController *pController ) override;
	IPhysicsPlayerController *CreatePlayerController( IPhysicsObject *pObject ) override;
	void DestroyPlayerController( IPhysicsPlayerController *pController ) override;
	IPhysicsVehicleController *CreateVehicleController( IPhysicsObject *pVehicleBodyObject, const vehicleparams_t &params, unsigned int nVehicleType, IPhysicsGameTrace *pGameTrace ) override;
	void DestroyVehicleController( IPhysicsVehicleController *pController ) override;

	void SetQuickDelete( bool bQuick ) override
	{
		m_deleteQuick = bQuick;
	}
	virtual bool ShouldQuickDelete() const { return m_deleteQuick; }
	virtual void TraceBox( trace_t *ptr, const Vector &mins, const Vector &maxs, const Vector &start, const Vector &end );
	void SetCollisionSolver( IPhysicsCollisionSolver *pCollisionSolver ) override;
	void GetGravity( Vector *pGravityVector ) const override;
	int	 GetActiveObjectCount() const override;
	void GetActiveObjects( IPhysicsObject **pOutputObjectList ) const override;
	const IPhysicsObject **GetObjectList( intp *pOutputObjectCount ) const override;
	bool TransferObject( IPhysicsObject *pObject, IPhysicsEnvironment *pDestinationEnvironment ) override;

	IVP_Environment	*GetIVPEnvironment( void ) { return m_pPhysEnv; }
	void		ClearDeadObjects( void );
	IVP_Controller *GetDragController() { return m_pDragController; }
	const IVP_Controller *GetDragController() const { return m_pDragController; }
	void SetAirDensity( float density ) override;
	float GetAirDensity( void ) const override;
	void ResetSimulationClock( void ) override;
	IPhysicsObject *CreateSphereObject( float radius, int materialIndex, const Vector &position, const QAngle &angles, objectparams_t *pParams, bool isStatic ) override;
	void CleanupDeleteList() override;
	void EnableDeleteQueue( bool enable ) override { m_queueDeleteObject = enable; }
	// debug
	bool IsCollisionModelUsed( CPhysCollide *pCollide ) const override;

	// trace against the physics world
	void TraceRay( const Ray_t &ray, unsigned int fMask, IPhysicsTraceFilter *pTraceFilter, trace_t *pTrace ) override;
	void SweepCollideable( const CPhysCollide *pCollide, const Vector &vecAbsStart, const Vector &vecAbsEnd, 
		const QAngle &vecAngles, unsigned int fMask, IPhysicsTraceFilter *pTraceFilter, trace_t *pTrace ) override;

	// performance tuning
	void GetPerformanceSettings( physics_performanceparams_t *pOutput ) const override;
	void SetPerformanceSettings( const physics_performanceparams_t *pSettings ) override;

	// perf/cost statistics
	void ReadStats( physics_stats_t *pOutput ) override;
	void ClearStats() override;
	void EnableConstraintNotify( bool bEnable ) override;
	// debug
	void DebugCheckContacts(void) override;

	// Save/restore
	bool Save( const physsaveparams_t &params  ) override;
	void PreRestore( const physprerestoreparams_t &params ) override;
	bool Restore( const physrestoreparams_t &params ) override;
	void PostRestore() override;
	void PhantomAdd( CPhysicsObject *pObject );
	void PhantomRemove( CPhysicsObject *pObject );

	void AddPlayerController( IPhysicsPlayerController *pController );
	void RemovePlayerController( IPhysicsPlayerController *pController );
	IPhysicsPlayerController *FindPlayerController( IPhysicsObject *pObject );

	IPhysicsCollisionEvent *GetCollisionEventHandler();
	// a constraint is being disabled - report the game DLL as "broken"
	void NotifyConstraintDisabled( IPhysicsConstraint *pConstraint );

private:
	IVP_Environment					*m_pPhysEnv;
	IVP_Controller					*m_pDragController;
	IVPhysicsDebugOverlay			*m_pDebugOverlay;			// Interface to use for drawing debug overlays.
	CUtlVector<IPhysicsObject *>	m_objects;
	CUtlVector<IPhysicsObject *>	m_deadObjects;
	CUtlVector<CPhysicsFluidController *> m_fluids;
	CUtlVector<IPhysicsPlayerController *> m_playerControllers;
	CSleepObjects					*m_pSleepEvents;
	CPhysicsListenerCollision		*m_pCollisionListener;
	CCollisionSolver				*m_pCollisionSolver;
	CPhysicsListenerConstraint		*m_pConstraintListener;
	CDeleteQueue					*m_pDeleteQueue;
	intp							m_lastObjectThisTick;
	bool							m_deleteQuick;
	bool							m_inSimulation;
	bool							m_queueDeleteObject;
	bool							m_fixedTimestep;
	bool							m_enableConstraintNotify;
};

extern IPhysicsEnvironment *CreatePhysicsEnvironment( void );

class IVP_Synapse_Friction;
class IVP_Real_Object;
extern IVP_Real_Object *GetOppositeSynapseObject( IVP_Synapse_Friction *pfriction );
extern IPhysicsObjectPairHash *CreateObjectPairHash();

#endif // PHYSICS_WORLD_H
