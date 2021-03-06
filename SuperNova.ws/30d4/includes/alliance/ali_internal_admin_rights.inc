<?php

if(!defined('SN_IN_ALLY') || SN_IN_ALLY !== true)
{
  $debug->error("Attempt to call ALLIANCE page mode {$mode} directly - not from alliance.php", 'Forbidden', 403);
}

if (!$user_admin) {
  message($lang['Denied_access'], $lang['ali_adm_rights_title']);
};

$template = gettemplate('ali_admin_rights', true);

$new_rank_name = sys_get_param_str('newRankName');
if ($new_rank_name)
{
  foreach($rights as $fieldName)
  {
    $newRank[$fieldName] = 0;
  }
  $newRank['name'] = $new_rank_name;

  $ranks[] = $newRank;
}

$rankListInput = sys_get_param('u');
if (is_array($rankListInput))
{
  unset($ranks);

  foreach($rankListInput as $rankID => $rank)
  {
    foreach($rights as $rightName)
    {
      $ranks[$rankID][$rightName] = $rank[$rightName] ? 1 : 0;
    }
    $ranks[$rankID]['name'] = strip_tags($rank['name']);
  }
}

$d = sys_get_param_int('d');
if ($d && isset($ranks[$d]))
{
  if(count($ranks) == 1)
  {
    message($lang['ali_adm_lastRank'], $lang['ali_adm_rights_title']);
  }
  array_splice($ranks, $d, 1);
  doquery("UPDATE {{users}} SET `ally_rank_id`=`ally_rank_id` - 1 WHERE `ally_id`={$ally['id']} AND `ally_rank_id` >= {$d}");
}

ALI_rankListSave($ranks);

if (count($ranks))
{
  foreach($ranks as $rankID => $rank)
  {
    $rank_data = array(
      'ID'   => $rankID,
      'NAME' => $rank['name'],
    );

    for($i = 1; $i < count($rank); $i++)
    {
      $rank_data['R' . $i] = (($rank[$rights[$i]] == 1) ? ' checked' : '') ;
      $rank_data['N' . $i] = $rights[$i];
    }

    $template->assign_block_vars('rank', $rank_data);
  }
}

display(parsetemplate($template), $lang['ali_adm_rights_title']);

?>
