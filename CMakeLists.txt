cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(REDENGINE)

include_directories(
  src
  src/pellet
)

if(WIN32)

	include_directories(
	contrib/windows-cl-x86_64/include
	)

	link_directories(
	contrib/windows-cl-x86_64/lib 
	)

	set(LIBOPENGL opengl32)
	set(LIBGLEW glew32)
	set(LIBOPENAL openal32)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	
	include_directories(
    contrib/emcc-emscripten-wasm/include
  )

  link_directories(
    contrib/emcc-emscripten-wasm/lib
  )

	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sALLOW_MEMORY_GROWTH")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/data@data")
	set(LIBOPENGL GL)
	set(LIBGLEW GLEW)
	set(LIBOPENAL AL)

endif()

add_library(rend
  src/rend/Animation.cpp
  src/rend/Animation.h
  src/rend/Buffer.cpp
  src/rend/Buffer.h
  src/rend/Debug.cpp
  src/rend/Debug.h
  src/rend/Exception.h
  src/rend/Face.h
  src/rend/Font.cpp
  src/rend/Font.h
  src/rend/Mesh.cpp
  src/rend/Mesh.h
  src/rend/Model.cpp
  src/rend/Model.h
  src/rend/ModelShader.cpp
  src/rend/ModelShader.h
  src/rend/RenderTexture.cpp
  src/rend/RenderTexture.h
  src/rend/Renderer.h
  src/rend/Shader.cpp
  src/rend/Shader.h
  src/rend/TextShader.cpp
  src/rend/TextShader.h
  src/rend/Texture.cpp
  src/rend/Texture.h
  src/rend/rend.h
  src/rend/rendfltk.h
  src/rend/rendgl.h
  src/rend/rendmath.cpp
  src/rend/rendmath.h
  src/rend/rendsdl.h
  src/rend/stb_image.h
  src/rend/utils.cpp
  src/rend/utils.h
)
target_link_libraries(rend SDL2 ${LIBGLEW} ${LIBOPENGL} ${LIBOPENAL})

add_library(pellet
  src/pellet/Bullet3Collision/BroadPhaseCollision/b3DynamicBvh.cpp
  src/pellet/Bullet3Collision/BroadPhaseCollision/b3DynamicBvhBroadphase.cpp
  src/pellet/Bullet3Collision/BroadPhaseCollision/b3OverlappingPairCache.cpp
  src/pellet/Bullet3Collision/NarrowPhaseCollision/b3ConvexUtility.cpp
  src/pellet/Bullet3Collision/NarrowPhaseCollision/b3CpuNarrowPhase.cpp
  src/pellet/Bullet3Common/b3AlignedAllocator.cpp
  src/pellet/Bullet3Common/b3Logging.cpp
  src/pellet/Bullet3Common/b3Vector3.cpp
  src/pellet/Bullet3Dynamics/b3CpuRigidBodyPipeline.cpp
  src/pellet/Bullet3Dynamics/ConstraintSolver/b3FixedConstraint.cpp
  src/pellet/Bullet3Dynamics/ConstraintSolver/b3Generic6DofConstraint.cpp
  src/pellet/Bullet3Dynamics/ConstraintSolver/b3PgsJacobiSolver.cpp
  src/pellet/Bullet3Dynamics/ConstraintSolver/b3Point2PointConstraint.cpp
  src/pellet/Bullet3Dynamics/ConstraintSolver/b3TypedConstraint.cpp
  src/pellet/Bullet3Geometry/b3ConvexHullComputer.cpp
  src/pellet/Bullet3Geometry/b3GeometryUtil.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btDbvt.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btDispatcher.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp
  src/pellet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp
  src/pellet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp
  src/pellet/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
  src/pellet/BulletCollision/CollisionDispatch/btCollisionObject.cpp
  src/pellet/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
  src/pellet/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp
  src/pellet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
  src/pellet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btGhostObject.cpp
  src/pellet/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp
  src/pellet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
  src/pellet/BulletCollision/CollisionDispatch/btManifoldResult.cpp
  src/pellet/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp
  src/pellet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp
  src/pellet/BulletCollision/CollisionDispatch/btUnionFind.cpp
  src/pellet/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp
  src/pellet/BulletCollision/CollisionShapes/btBox2dShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btBoxShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btCapsuleShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btCollisionShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btCompoundShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btConcaveShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btConeShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btConvex2dShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btConvexHullShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp
  src/pellet/BulletCollision/CollisionShapes/btConvexShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btCylinderShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btEmptyShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btMultiSphereShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btOptimizedBvh.cpp
  src/pellet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btShapeHull.cpp
  src/pellet/BulletCollision/CollisionShapes/btSphereShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp
  src/pellet/BulletCollision/CollisionShapes/btTetrahedronShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btTriangleBuffer.cpp
  src/pellet/BulletCollision/CollisionShapes/btTriangleCallback.cpp
  src/pellet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp
  src/pellet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp
  src/pellet/BulletCollision/CollisionShapes/btTriangleMesh.cpp
  src/pellet/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp
  src/pellet/BulletCollision/CollisionShapes/btUniformScalingShape.cpp
  src/pellet/BulletCollision/Gimpact/btContactProcessing.cpp
  src/pellet/BulletCollision/Gimpact/btGenericPoolAllocator.cpp
  src/pellet/BulletCollision/Gimpact/btGImpactBvh.cpp
  src/pellet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp
  src/pellet/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp
  src/pellet/BulletCollision/Gimpact/btGImpactShape.cpp
  src/pellet/BulletCollision/Gimpact/btTriangleShapeEx.cpp
  src/pellet/BulletCollision/Gimpact/gim_box_set.cpp
  src/pellet/BulletCollision/Gimpact/gim_contact.cpp
  src/pellet/BulletCollision/Gimpact/gim_memory.cpp
  src/pellet/BulletCollision/Gimpact/gim_tri_collision.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
  src/pellet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp
  src/pellet/BulletDynamics/Character/btKinematicCharacterController.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btContactConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btGearConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp
  src/pellet/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp
  src/pellet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
  src/pellet/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.cpp
  src/pellet/BulletDynamics/Dynamics/btRigidBody.cpp
  src/pellet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp
  src/pellet/BulletDynamics/Dynamics/btSimulationIslandManagerMt.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBody.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBodyConstraint.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBodyFixedConstraint.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBodyJointMotor.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBodyPoint2Point.cpp
  src/pellet/BulletDynamics/Featherstone/btMultiBodySliderConstraint.cpp
  src/pellet/BulletDynamics/MLCPSolvers/btDantzigLCP.cpp
  src/pellet/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.cpp
  src/pellet/BulletDynamics/MLCPSolvers/btMLCPSolver.cpp
  src/pellet/BulletDynamics/Vehicle/btRaycastVehicle.cpp
  src/pellet/BulletDynamics/Vehicle/btWheelInfo.cpp
  src/pellet/BulletInverseDynamics/details/MultiBodyTreeImpl.cpp
  src/pellet/BulletInverseDynamics/details/MultiBodyTreeInitCache.cpp
  src/pellet/BulletInverseDynamics/IDMath.cpp
  src/pellet/BulletInverseDynamics/MultiBodyTree.cpp
  src/pellet/BulletSoftBody/btDefaultSoftBodySolver.cpp
  src/pellet/BulletSoftBody/btSoftBody.cpp
  src/pellet/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp
  src/pellet/BulletSoftBody/btSoftBodyHelpers.cpp
  src/pellet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp
  src/pellet/BulletSoftBody/btSoftMultiBodyDynamicsWorld.cpp
  src/pellet/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp
  src/pellet/BulletSoftBody/btSoftRigidDynamicsWorld.cpp
  src/pellet/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp
  src/pellet/LinearMath/btAlignedAllocator.cpp
  src/pellet/LinearMath/btConvexHull.cpp
  src/pellet/LinearMath/btConvexHullComputer.cpp
  src/pellet/LinearMath/btGeometryUtil.cpp
  src/pellet/LinearMath/btPolarDecomposition.cpp
  src/pellet/LinearMath/btQuickprof.cpp
  src/pellet/LinearMath/btSerializer.cpp
  src/pellet/LinearMath/btThreads.cpp
  src/pellet/LinearMath/btVector3.cpp
)

add_library(RedEngine
 src/RedEngine/RedEngine.h
 src/RedEngine/Core.cpp
 src/RedEngine/Core.h
 src/RedEngine/Entity.h
 src/RedEngine/Entity.cpp
 src/RedEngine/Component.h
 src/RedEngine/Component.cpp
 src/RedEngine/Window.h
 src/RedEngine/Window.cpp
 src/RedEngine/TriangleRenderer.h
 src/RedEngine/TriangleRenderer.cpp
 src/RedEngine/Resources.h
 src/RedEngine/Resource.h
 src/RedEngine/Texture.h
 src/RedEngine/Model.h 
 src/RedEngine/Model.cpp 
 src/RedEngine/Shader.h 
 src/RedEngine/AudioClip.h
 src/RedEngine/AudioClip.cpp 
 src/RedEngine/Textfile.h 
 src/RedEngine/Map.h 
 src/RedEngine/ModelRenderer.h 
 src/RedEngine/ModelRenderer.cpp 
 src/RedEngine/Transform.h 
 src/RedEngine/Transform.cpp 
 src/RedEngine/Input.h
 src/RedEngine/Input.cpp
 src/RedEngine/BoxCollider.h
 src/RedEngine/BoxCollider.cpp
 src/RedEngine/RigidBody.h
 src/RedEngine/RigidBody.cpp
 src/RedEngine/Collider.h
 src/RedEngine/Collider.cpp
 src/RedEngine/GUI.h
 src/RedEngine/GUI.cpp
 src/RedEngine/AudioSource.h 
 src/RedEngine/AudioSource.cpp 
 src/RedEngine/MeshCollider.h 
 src/RedEngine/MeshCollider.cpp 
 src/RedEngine/Camera.h 
 src/RedEngine/Camera.cpp 
 src/RedEngine/Spline.h
 src/RedEngine/Spline.cpp
 src/RedEngine/SplineRenderer.h 
 src/RedEngine/SplineRenderer.cpp
)

add_library(IMGUI
 src/IMGUI/imconfig.h 
 src/IMGUI/imgui.cpp 
 src/IMGUI/imgui.h 
 src/IMGUI/imgui_draw.cpp 
 src/IMGUI/imgui_impl_opengl3.cpp 
 src/IMGUI/imgui_impl_opengl3.h 
 src/IMGUI/imgui_impl_opengl3_loader.h 
 src/IMGUI/imgui_impl_sdl.cpp 
 src/IMGUI/imgui_impl_sdl.h 
 src/IMGUI/imgui_impl_sdlrenderer.cpp 
 src/IMGUI/imgui_impl_sdlrenderer.h 
 src/IMGUI/imgui_internal.h 
 src/IMGUI/imgui_tables.cpp 
 src/IMGUI/imgui_widgets.cpp 
 src/IMGUI/imstb_rectpack.h 
 src/IMGUI/imstb_textedit.h 
 src/IMGUI/imstb_truetype.h
)



target_link_libraries(RedEngine rend pellet IMGUI)



add_executable(demo
  src/demo/main.cpp
  src/demo/Menu.h 
  src/demo/Menu.cpp 
  src/demo/FollowCam.h 
  src/demo/FollowCam.cpp 
  src/demo/CarPhysics.h 
  src/demo/CarPhysics.cpp 
  src/demo/Track.h
  src/demo/Track.cpp
)

target_link_libraries(demo RedEngine)
