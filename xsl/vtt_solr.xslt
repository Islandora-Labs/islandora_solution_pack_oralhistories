<?xml version="1.0" encoding="UTF-8"?>
<!-- ORALHISTORIES TRANSCRIPT -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:foxml="info:fedora/fedora-system:def/foxml#" xmlns:dcterms="http://purl.org/dc/terms/">
  <xsl:template match="foxml:datastream[contains(@ID, 'INDEXMEDIATRACK') or contains(@ID, 'INDEXTRANSCRIPT')]/foxml:datastreamVersion[last()]" name="index_VTT">
    <xsl:param name="prefix">vtt_</xsl:param>
    <xsl:param name="content"/>    
    <field>
      <xsl:attribute name="name">
        <xsl:value-of select="concat($prefix, 'content')"/>
      </xsl:attribute>
      <xsl:value-of select="$content"/>
     </field>     
  </xsl:template>
</xsl:stylesheet>
