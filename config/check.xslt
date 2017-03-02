<?xml version="1.0"?>

<xsl:stylesheet version="1.0" 
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/TR/xhtml1/strict">

<xsl:output method="text"/>

<xsl:strip-space elements="*"/>

<xsl:template match="optiondepend">

###################################################  

  <xsl:for-each select="single[depend]/option">
    <xsl:call-template name="depends"/>
  </xsl:for-each>

  <xsl:for-each select="group[depend]/option">
    <xsl:call-template name="depends"/>
  </xsl:for-each>

  <xsl:for-each select="andgroup[depend]/option">
    <xsl:call-template name="anddepends"/>
  </xsl:for-each>

###################################################  

  <xsl:for-each select="single[conflict]/option">
    <xsl:call-template name="conflicts"/>
  </xsl:for-each>

  <xsl:for-each select="group[conflict]/option">
    <xsl:call-template name="conflicts"/>
  </xsl:for-each>

  <xsl:for-each select="andgroup[conflict]/option">
    <xsl:call-template name="andconflicts"/>
  </xsl:for-each>

###################################################  

  <xsl:for-each select="single[code|dir]/option">
    <xsl:call-template name="codedirs"/>
  </xsl:for-each>

  <xsl:for-each select="group[code|dir]/option">
    <xsl:call-template name="codedirs"/>
  </xsl:for-each>

  <xsl:for-each select="andgroup[code|dir]/option">
    <xsl:call-template name="andcodedirs"/>
  </xsl:for-each>

</xsl:template>



<xsl:template name="depends" match="single/option|group/option">

# dependencies for __<xsl:value-of select="."/>__
ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
  <xsl:for-each select="../depend">
  ifneq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
    ERIKAOPT += __<xsl:value-of select="."/>__
  endif
  </xsl:for-each>
endif

</xsl:template>



<xsl:template name="anddepends" match="andgroup">

# dependencies for group 
<xsl:for-each select="option">
ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
</xsl:for-each>
  <xsl:for-each select="depend">
  ifneq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
    ERIKAOPT += __<xsl:value-of select="."/>__
  endif
  </xsl:for-each>
<xsl:for-each select="option">
endif
</xsl:for-each>

</xsl:template>




<xsl:template name="conflicts" match="single/option|group/option">

# conflicts for __<xsl:value-of select="."/>__
ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
  <xsl:for-each select="../conflict">
  ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
    $(error __<xsl:value-of select="../option"/>__ and __<xsl:value-of select="."/>__ are not compatible )
  endif
  </xsl:for-each>
endif

</xsl:template>



<xsl:template name="andconflicts" match="andgroup">

# conflicts for group 
<xsl:for-each select="option">
ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
</xsl:for-each>
  <xsl:for-each select="conflict">
  ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
    $(error __<xsl:value-of select="../option"/>__ and __<xsl:value-of select="."/>__ are not compatible )
  endif
  </xsl:for-each>
<xsl:for-each select="option">
endif
</xsl:for-each>

</xsl:template>



<xsl:template name="code" match="single/option|group/option">

# code and directories for __<xsl:value-of select="."/>__
ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
  <xsl:for-each select="../code">
  ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
    $(error __<xsl:value-of select="../option"/>__ and __<xsl:value-of select="."/>__ are not compatible )
  endif
  </xsl:for-each>
endif

</xsl:template>



<xsl:template name="andconflicts" match="andgroup">

# conflicts for group 
<xsl:for-each select="option">
ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
</xsl:for-each>
  <xsl:for-each select="conflict">
  ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
    $(error __<xsl:value-of select="../option"/>__ and __<xsl:value-of select="."/>__ are not compatible )
  endif
  </xsl:for-each>
<xsl:for-each select="option">
endif
</xsl:for-each>

</xsl:template>



<xsl:template name="codedirs" match="single/option|group/option">

# code and directories for __<xsl:value-of select="."/>__
ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
  <xsl:if test="../code">
  CODE += <xsl:for-each select="../code"><xsl:value-of select="."/> </xsl:for-each>
  </xsl:if>
  <xsl:if test="../dir">
  DIR += <xsl:for-each select="../dir"><xsl:value-of select="."/> </xsl:for-each>
  </xsl:if>
  <xsl:if test="../lib">
  LIBS += <xsl:for-each select="../lib"><xsl:value-of select="."/> </xsl:for-each>
  </xsl:if>
  <xsl:if test="../libdir">
  LIBRARY_PATH += <xsl:for-each select="../libdir"><xsl:value-of select="."/> </xsl:for-each>
  </xsl:if>
  <xsl:if test="../incdir">
  INCLUDE += <xsl:for-each select="../inc"><xsl:value-of select="."/> </xsl:for-each>
  </xsl:if>
endif

</xsl:template>



<xsl:template name="andcodedirs" match="andgroup">

# code and directories for group 
<xsl:for-each select="option">
ifeq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
</xsl:for-each>
<xsl:for-each select="nooption">
ifneq ($(findstring __<xsl:value-of select="."/>__,$(ERIKAOPT)), __<xsl:value-of select="."/>__)
</xsl:for-each>
  <xsl:if test="../code">
  CODE += <xsl:for-each select="../code"><xsl:value-of select="."/> </xsl:for-each>
  </xsl:if>
  <xsl:if test="../dir">
  DIR += <xsl:for-each select="../dir"><xsl:value-of select="."/> </xsl:for-each>
  </xsl:if>
<xsl:for-each select="option|nooption">
endif
</xsl:for-each>

</xsl:template>

 
</xsl:stylesheet>
