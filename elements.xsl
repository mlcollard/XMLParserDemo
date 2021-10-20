<xsl:stylesheet xmlns="http://www.srcML.org/srcML/src"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    version="1.0">

<!-- 
    elements.xsl

    Output all element names
 -->

<xsl:output method="text"/>

<xsl:template match="src:* | cpp:*"><xsl:copy-of select="local-name()"/><xsl:text>
</xsl:text>
    <xsl:apply-templates select="src:* | cpp:*"/>
</xsl:template>

<xsl:template match="@*|node()">
    <xsl:apply-templates select="@*|node()"/>
</xsl:template>

<!-- Canonical copy -->
<xsl:template match="@*|node()" mode="keep">
    <xsl:copy>
        <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
</xsl:template>

</xsl:stylesheet>
