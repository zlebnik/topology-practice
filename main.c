/* Program usage: mpirun -np 1 rosenbrock1 [-help] [all TAO options] */

/*  Include "tao.h" so we can use TAO solvers.  */
#include "tao.h"
#include "placement.h"
#include "functions.h"
#include "rectshape.h"
#include "output.h"

#include <iostream>

using namespace std;

static  char help[] = "This example demonstrates use of the TAO package to \n\
solve an unconstrained minimization problem on a single processor.  We \n\
minimize the extended Rosenbrock function: \n\
   sum_{i=0}^{n/2-1} ( alpha*(x_{2i+1}-x_{2i}^2)^2 + (1-x_{2i})^2 ) \n";

/*T 
   Concepts: TAO - Solving an unconstrained minimization problem
   Routines: TaoInitialize(); TaoFinalize();
   Routines: TaoCreate();
   Routines: TaoSetType(); TaoSetObjectiveAndGradientRoutine();
   Routines: TaoSetHessianRoutine();
   Routines: TaoSetInitialVector();
   Routines: TaoSetFromOptions();
   Routines: TaoSolve();
   Routines: TaoGetTerminationReason(); TaoDestroy(); 
   Processors: 1
T*/ 


/* 
   User-defined application context - contains data needed by the 
   application-provided call-back routines that evaluate the function,
   gradient, and hessian.
*/
typedef struct {
  PetscInt n;          /* dimension */
  PetscReal alpha;   /* condition parameter */
  PetscReal precision;
} AppCtx;

Placement* placement = new Placement();
Functions* functions;

/* -------------- User-defined routines ---------- */
PetscErrorCode FormFunctionGradient(TaoSolver,Vec,PetscReal*,Vec,void*);
//PetscErrorCode FormHessian(TaoSolver,Vec,Mat*,Mat*,MatStructure*,void*);

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **argv)
{
  PetscErrorCode  ierr;                  /* used to check for functions returning nonzeros */
  PetscReal zero=0.0;
  Vec        x;                     /* solution vector */
  //Mat        H;
  TaoSolver  tao;                   /* TaoSolver solver context */
  PetscBool  flg;
  int        size,rank;                  /* number of processes running */
  TaoSolverTerminationReason reason;
  AppCtx     user;                  /* user-defined application context */

  /* Initialize TAO and PETSc */
  TaoInitialize(&argc,&argv,(char*)0,help);
  ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size); CHKERRQ(ierr);
  ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank); CHKERRQ(ierr);

  /* input part */
  int shapesNumber;
  cin >> shapesNumber;
  int i;

  for (i = 0; i < shapesNumber; i++) {
    double x, y, offsetX, offsetY;
    cin >> x >> y >> offsetX >> offsetY;
    placement->addShape(new RectShape(x, y, offsetX, offsetY));
  }

  user.n = placement->getVariablesLength();
  /* end of input */

  /* Initialize problem parameters */
  user.alpha = 0;
  user.precision = 0.1;
  /* Check for command line arguments to override defaults */
  ierr = PetscOptionsGetReal(PETSC_NULL,"--alpha",&user.alpha,&flg); CHKERRQ(ierr);
  ierr = PetscOptionsGetReal(PETSC_NULL,"--precision",&user.precision,&flg); CHKERRQ(ierr);
  functions = new Functions(user.precision);

  /* Allocate vectors for the solution and gradient */
  ierr = VecCreateSeq(PETSC_COMM_SELF,user.n,&x); CHKERRQ(ierr);

  /* The TAO code begins here */

  /* Create TAO solver with desired solution method */
  ierr = TaoCreate(PETSC_COMM_SELF,&tao); CHKERRQ(ierr);
  ierr = TaoSetType(tao,"tao_lmvm"); CHKERRQ(ierr);

  /* Set solution vec and an initial guess */
  VecSet(x, zero);
  double *variables = placement->getVariables();
  placement->getGradient(user.alpha);
  for (i = 0; i < user.n; i++) {
    VecSetValue(x, i, variables[i], ADD_VALUES);
  }

  

  ierr = TaoSetInitialVector(tao,x); CHKERRQ(ierr); 

  /* Set routines for function, gradient, hessian evaluation */
  ierr = TaoSetObjectiveAndGradientRoutine(tao,FormFunctionGradient,&user); CHKERRQ(ierr);

  /* Check for TAO command line options */
  ierr = TaoSetFromOptions(tao); CHKERRQ(ierr);

  /* SOLVE THE APPLICATION */
  int attempts = 0;
  do {
    ierr = TaoSetInitialVector(tao,x); CHKERRQ(ierr); 
    ierr = TaoSolve(tao); CHKERRQ(ierr);

    /* Get termination information */
    ierr = TaoGetTerminationReason(tao,&reason); CHKERRQ(ierr);

    user.alpha = user.alpha * 6 / 7;
    attempts++;
  } while (user.alpha > 0.000001 && reason <= 0);
  if (functions->getOverlap(placement) > 0) {
    ierr = TaoSetInitialVector(tao,x); CHKERRQ(ierr); 
    user.alpha = 0;
    ierr = TaoSolve(tao); CHKERRQ(ierr);
    ierr = TaoGetTerminationReason(tao,&reason); CHKERRQ(ierr);
  }
  cerr << "Overlap: " << functions->getOverlap(placement) << endl;
  
  if (reason <= 0) {
    cerr << "Reason: " << reason << endl;
    cerr << "Try a different TAO type, adjust some parameters, or check the function evaluation routines\n";
  }
  cout << Output::toSVG(placement) << endl;
  /* Free TAO data structures */
  ierr = TaoDestroy(&tao); CHKERRQ(ierr);

  /* Free PETSc data structures */
  ierr = VecDestroy(&x); CHKERRQ(ierr);

  TaoFinalize();

  return 0;
}

/* -------------------------------------------------------------------- */
#undef __FUNCT__
#define __FUNCT__ "FormFunctionGradient"
/*  
    FormFunctionGradient - Evaluates the function, f(X), and gradient, G(X). 

    Input Parameters:
.   tao  - the TaoSolver context
.   X    - input vector
.   ptr  - optional user-defined context, as set by TaoSetFunctionGradient()
    
    Output Parameters:
.   G - vector containing the newly evaluated gradient
.   f - function value

    Note:
    Some optimization methods ask for the function and the gradient evaluation
    at the same time.  Evaluating both at once may be more efficient that
    evaluating each separately. 
*/
PetscErrorCode FormFunctionGradient(TaoSolver tao,Vec X,PetscReal *f, Vec G,void *ptr)
{
  AppCtx *user = (AppCtx *) ptr;  
  PetscErrorCode ierr;
  PetscReal *x,*g;
  double area, overlap;

  /* Get pointers to vector data */
  ierr = VecGetArray(X,&x); CHKERRQ(ierr);
  ierr = VecGetArray(G,&g); CHKERRQ(ierr);

  /* Compute G(X) */
  placement->update(x);
  placement->getGradient(user->alpha, g, user->precision);
  

  area = functions->getSmoothArea(placement);
  overlap = functions->getSmoothOverlap(placement);

  *f = user->alpha * area + (1 - user->alpha) * overlap;
  /*cerr << "V:";
  for (int i = 0; i < user->n; i++) {
    cerr << " " << x[i];
  }
  cerr << endl;

  cerr << "f: " << user->alpha << '*' << area << " + " << 1 - user->alpha << "*" << overlap << " = " << *f << endl;
  
  cerr << "G:";
  for (int i = 0; i < user->n; i++) {
    cerr << " " << g[i];
  }
  cerr << endl << endl;*/

  /* Restore vectors */
  ierr = VecRestoreArray(X,&x); CHKERRQ(ierr);
  ierr = VecRestoreArray(G,&g); CHKERRQ(ierr);
  
  return 0;
}

/* ------------------------------------------------------------------- */
#undef __FUNCT__
