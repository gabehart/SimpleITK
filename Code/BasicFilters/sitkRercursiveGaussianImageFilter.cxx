#include "sitkRecursiveGaussianImageFilter.h"

namespace itk {
namespace simple {

//----------------------------------------------------------------------------

//
// Default constructor that initializes parameters
//
RecursiveGaussianImageFilter::RecursiveGaussianImageFilter ()
  {
  this->m_Sigma = 1.0;
  this->m_Order = ZeroOrder;
  this->m_NormalizeAcrossScale = false;

  this->m_MemberFactory.reset( new detail::MemberFunctionFactory<Self>( this ) );

  this->m_MemberFactory->RegisterMemberFunctions< PixelTypeList, 3 > ();
  this->m_MemberFactory->RegisterMemberFunctions< PixelTypeList, 2 > ();
  }

//
// ToString
//
std::string RecursiveGaussianImageFilter::ToString()
  {
  std::ostringstream out;
  out << "itk::simple::RecursiveGaussianImageFilter\n"
      << "\tSigma: " << this->m_Sigma << "\n"
      << "\tOrder: " << this->m_Order << "\n"
      << "\tNormalizeAcrossScale: " << this->m_NormalizeAcrossScale << "\n";
  return out.str();
  }

//
// SetSigma
//
RecursiveGaussianImageFilter& RecursiveGaussianImageFilter::SetSigma ( double sigma )
  {
  this->m_Sigma = sigma;
  return *this;
  }

//
// GetSigma
//
double RecursiveGaussianImageFilter::GetSigma()
  {
  return this->m_Sigma;
  }

//
// SetNormalizeAcrossScale
//
RecursiveGaussianImageFilter&
RecursiveGaussianImageFilter::SetNormalizeAcrossScale ( bool normalizeAcrossScale )
  {
  this->m_NormalizeAcrossScale = normalizeAcrossScale;
  return *this;
  }

//
// GetNormalizeAcrossScale
//
bool RecursiveGaussianImageFilter::GetNormalizeAcrossScale()
  {
  return this->m_NormalizeAcrossScale;
  }

//
// SetOrder
//
RecursiveGaussianImageFilter&
RecursiveGaussianImageFilter::SetOrder( OrderEnumType order )
  {
  this->m_Order = order;
  return *this;
  }

//
// GetSigma
//
RecursiveGaussianImageFilter::OrderEnumType RecursiveGaussianImageFilter::GetOrder()
  {
  return this->m_Order;
  }

//----------------------------------------------------------------------------

//
// Execute
//
Image::Pointer RecursiveGaussianImageFilter::Execute ( Image::Pointer image )
  {

  int fnIndex = image->GetImageDataType();

  // todo fix this ugly syntax
  return NULL;//((*this).*(m_MemberFactory.GetMemberFunction( fnIndex )))(image);
  }

//----------------------------------------------------------------------------

//
// ExecuteInternal
//
template <class TImageType>
Image::Pointer RecursiveGaussianImageFilter::ExecuteInternal ( Image::Pointer inImage )
  {
  typedef TImageType InputImageType;
  typedef itk::Image<float,InputImageType::ImageDimension> OutputImageType;
  typename InputImageType::Pointer image =
    dynamic_cast <InputImageType*> ( inImage->GetImageBase().GetPointer() );

  if ( image.IsNull() ) {
    // Take some action
    return NULL;
  }

  typedef itk::RecursiveGaussianImageFilter<InputImageType, OutputImageType> GaussianFilterType;
  typename GaussianFilterType::Pointer filter = GaussianFilterType::New();

  filter->SetInput ( image );
  filter->SetSigma ( this->m_Sigma );
  filter->SetNormalizeAcrossScale ( this->m_NormalizeAcrossScale );

  // cast the order parameter to the type from the itk filter
  typename GaussianFilterType::OrderEnumType internalOrder =
    static_cast<typename GaussianFilterType::OrderEnumType>(this->m_Order);
  filter->SetOrder ( internalOrder );

  filter->Update();

  Image::Pointer out = new Image( filter->GetOutput() );
  filter->GetOutput()->DisconnectPipeline();
  return out;
  }

} // end namespace simple
} // end namespace itk
