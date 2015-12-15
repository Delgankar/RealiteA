#include <osgViewer/Viewer> 
#include <osg/Geometry> 
#include <osg/ShapeDrawable> 

osg::Node* creerLaScene() 
{ 
     // Nous cr�ons un objet Geometry dans lequel nous allons construire notre triangle. 
     osg::Geometry* geoTriangle = new osg::Geometry; 

     // Nous cr�ons un tableau de trois sommets. 
     osg::Vec3Array* tabSommet = new osg::Vec3Array; 
     tabSommet->push_back(osg::Vec3(-1, 0, -1)); 
     tabSommet->push_back(osg::Vec3(1, 0, -1)); 
     tabSommet->push_back(osg::Vec3(0, 0, 1)); 

     // Nous ajoutons le tableau de sommet a notre objet Geometry. 
     geoTriangle ->setVertexArray(tabSommet); 

     // Nous cr�ons une primitive Triangle et nous ajoutons les sommets selon leur index dans le tableau tabSommet 
     osg::DrawElementsUInt* pPrimitiveSet = 
     new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 ); 
     pPrimitiveSet->push_back(0); 
     pPrimitiveSet->push_back(1); 
     pPrimitiveSet->push_back(2); 

     // Nous ajoutons notre primitive a notre objet Geometry. 
     geoTriangle->addPrimitiveSet(pPrimitiveSet); 

     // On met en place un tableau de couleurs. Dans notre exemple chaque sommet du triangle aura une couleur diff�rente. 
     osg::Vec4Array* tabCouleur = new osg::Vec4Array; 
     tabCouleur->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); 
     tabCouleur->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); 
     tabCouleur->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); 
     geoTriangle->setColorArray(tabCouleur); 

     // Nous nous assurons que notre triangle utilisera bien une couleur par sommet. 
     geoTriangle->setColorBinding(osg::Geometry::BIND_PER_VERTEX); 

     /*---------------------------------/!\----------------------------------*/ 
     // Nous cr�ons un n�ud g�om�trique afin de stocker notre triangle et nous d�sactivons sa lumi�re. 
     osg::Geode* noeudGeo = new osg::Geode; 
     osg::StateSet* status = noeudGeo->getOrCreateStateSet(); 
     status->setMode(GL_LIGHTING, osg::StateAttribute::OFF); 
     noeudGeo->addDrawable(geoTriangle); 
     /*----------------------------------------------------------------------*/ 
     
     return noeudGeo; 
} 

int main(int argc, char* argv[]) 
{ 
     // Construction du Viewer. 
     osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer; 

     // Nous demandons au viewer de cr�er une fen�tre de 512x512 pixels que nous pla�ons � 50 pixels du bord haut gauche de notre �cran. 
     viewer->setUpViewInWindow(50, 50, 512, 512); 

     // Nous associons au viewer la sc�ne 3D dont nous voulons effectuer le rendu. 
     viewer->setSceneData(creerLaScene()); 

     // Nous ex�cutons la fonction run du viewer. 
     return viewer->run(); 
} 

