#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkImageViewer2.h>
#include <vtkJPEGReader.h>
#include <vtkAxes.h>
#include <vtkAxesActor.h>
#include <vtkMatrix4x4.h>
#include <math.h>

double square(double x)
{
	return x*x;
}

int main(int, char *[])
{
	vtkSmartPointer<vtkConeSource> coneX = vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkConeSource> coneY = vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkConeSource> coneZ = vtkSmartPointer<vtkConeSource>::New();
	coneX->SetRadius(0.04);
    coneX->SetDirection(1,0,0);
	coneX->SetCenter(coneX->GetHeight()/2.0,0,0);
	coneY->SetRadius(0.04);
	coneY->SetCenter(0,coneY->GetHeight()/2.0,0);
	coneY->SetDirection(0,1,0);
	coneZ->SetRadius(0.04);
	coneZ->SetDirection(0,0,1);
	coneZ->SetCenter(0,0,coneZ->GetHeight()/2.0);

  //Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapperX =  vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapperY =  vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapperZ =  vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperX->SetInputConnection(coneX->GetOutputPort());
    mapperY->SetInputConnection(coneY->GetOutputPort());
	mapperZ->SetInputConnection(coneZ->GetOutputPort());
    vtkSmartPointer<vtkActor> actorX = vtkSmartPointer<vtkActor>::New();
    actorX->SetMapper(mapperX);
    actorX->GetProperty()->SetColor(1,0,0);
	vtkSmartPointer<vtkActor> actorY = vtkSmartPointer<vtkActor>::New();
    actorY->SetMapper(mapperY);
    actorY->GetProperty()->SetColor(0,1,0);
	vtkSmartPointer<vtkActor> actorZ = vtkSmartPointer<vtkActor>::New();
    actorZ->SetMapper(mapperZ);
    actorZ->GetProperty()->SetColor(0,0,1);

  // Create a renderer, render window, and interactor
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	camera->SetPosition(0,0,10);
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetActiveCamera(camera);
	double cr[2];
	renderer->GetActiveCamera()->GetClippingRange(cr);
	cout << "Clipping Range:" << cr[1] << ":" << cr[2] << endl;
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(640,480);
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
	int windowSize[2];
	renderWindowInteractor->GetSize(windowSize[0], windowSize[1]);
	vtkSmartPointer<vtkMatrix4x4> extrinsicM = vtkSmartPointer<vtkMatrix4x4>::New();
	extrinsicM->DeepCopy(renderer->GetActiveCamera()->GetViewTransformMatrix());
	double orientation[3];
	camera->GetDirectionOfProjection(orientation);
	cout << "Orientation:" << endl;
	
	cout << orientation[0] << ":" << orientation[1] << ":" << orientation[2] << endl;
	camera->SetViewAngle(10);
	cout << "View Angle:" << camera->GetViewAngle() << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j=0; j<4; j++)
		{
			printf("%lf:\t",extrinsicM->GetElement(i,j));
		}
		cout << endl;
	}


	double thetaX, thetaY, thetaZ, c, yaw, pitch, roll;
	thetaX  = atan2(extrinsicM->GetElement(1,2), extrinsicM->GetElement(2,2));
	c = sqrt(square(extrinsicM->GetElement(0,0)) + square(extrinsicM->GetElement(0,1)));
	thetaY = atan2(-(extrinsicM->GetElement(0,2)), c);
	thetaZ = atan2(extrinsicM->GetElement(0,1), extrinsicM->GetElement(0,0));
	pitch = thetaX*180/3.14;
	yaw = thetaY*180/3.14;
	roll = thetaZ*180/3.14;

	cout <<"Roll:"<<roll << endl << "Pitch:" << pitch << endl << "Yaw:" << yaw << endl;
	

	
	//vtkSmartPointer<vtkMatrix4x4> projectorView = renderer->GetActiveCamera()->GetProjectionTransformMatrix(windowSize[0]/windowSize[1], cr[0], cr[1]);
	cout << "WindowSize:\nWidth:" << windowSize[0] << endl << "Height:" <<windowSize[1] << endl;
	double windowCentre[2];
	camera->GetWindowCenter(windowCentre);
	cout << "Window Centre:" << windowCentre[0] << ":" << windowCentre[1] << endl;
  // Add the actor to the scene
    renderer->AddActor(actorX);
	renderer->AddActor(actorY);
	renderer->AddActor(actorZ);
    renderer->SetBackground(1,1,1); 
 
  // Render and interact
    renderWindow->Render();
    renderWindowInteractor->Start();
 
    return EXIT_SUCCESS;
}