<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:stylesheet version="1.0" 
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/TR/xhtml1/strict">

<xsl:output method="text"/>

<xsl:template match="rtwoptions">
/%
  BEGIN_RTW_OPTIONS
  <xsl:for-each select="category[position()=1]">
    <xsl:call-template name="category">
      <xsl:with-param name="position">1</xsl:with-param>
    </xsl:call-template>
  </xsl:for-each>
  %----------------------------------------%
  % Configure RTW code generation settings %
  %----------------------------------------%
  
  rtwgensettings.BuildDirSuffix = '_ErikaEducational_rtw';

  END_RTW_OPTIONS
%/ 
</xsl:template>


<xsl:template name="category" match="category">
<xsl:param name="position"/>
  rtwoptions(<xsl:value-of select="$position"/>).prompt = '<xsl:value-of select="@name"/>';
  rtwoptions(<xsl:value-of select="$position"/>).type = 'Category';
  rtwoptions(<xsl:value-of select="$position"/>).enable = 'on';
  rtwoptions(<xsl:value-of select="$position"/>).default = '<xsl:value-of select="count(option)"/>';
  rtwoptions(<xsl:value-of select="$position"/>).popupstrings = '';
  rtwoptions(<xsl:value-of select="$position"/>).tlcvariable = '';
  rtwoptions(<xsl:value-of select="$position"/>).tooltip = '';
  rtwoptions(<xsl:value-of select="$position"/>).callback = '';
  rtwoptions(<xsl:value-of select="$position"/>).opencallback = '';
  rtwoptions(<xsl:value-of select="$position"/>).closecallback = '';
  rtwoptions(<xsl:value-of select="$position"/>).makevariable = '';

    <xsl:for-each select="option">
  rtwoptions(<xsl:value-of select="$position+position()"/>).prompt = '<xsl:value-of select="@prompt"/>';
  rtwoptions(<xsl:value-of select="$position+position()"/>).type = '<xsl:value-of select="@type"/>';<xsl:text></xsl:text>
<xsl:if test="makevariable">
  rtwoptions(<xsl:value-of select="$position+position()"/>).makevariable = '<xsl:value-of select="makevariable"/>';</xsl:if>
<xsl:if test="makevariable and not(tlcvariable)">
  rtwoptions(<xsl:value-of select="$position+position()"/>).tlcvariable = '<xsl:value-of select="makevariable"/>';</xsl:if>
<xsl:if test="tlcvariable">
  rtwoptions(<xsl:value-of select="$position+position()"/>).tlcvariable = '<xsl:value-of select="tlcvariable"/>';</xsl:if>
<xsl:if test="default">
  rtwoptions(<xsl:value-of select="$position+position()"/>).default = '<xsl:value-of select="default"/>';</xsl:if>
<xsl:if test="tooltip">
  rtwoptions(<xsl:value-of select="$position+position()"/>).tooltip = ['<xsl:value-of select="tooltip"/>'];</xsl:if>
<xsl:if test="popupstrings">
  rtwoptions(<xsl:value-of select="$position+position()"/>).popupstrings = '<xsl:value-of select="popupstrings"/>';</xsl:if><xsl:text>
</xsl:text>

    </xsl:for-each> 
  <xsl:variable name="currentposition" select="$position+count(option)+1"/>
  <xsl:for-each select="../category[position()=($position+1)]">
    <xsl:call-template name="category">
      <xsl:with-param name="position" select="$currentposition"/>
    </xsl:call-template>
  </xsl:for-each>

</xsl:template>





 
</xsl:stylesheet>
