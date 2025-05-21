#include "FollowersSummonsApplySpell.hpp"

#include "Core.hpp"

namespace Reflyem::FollowersSummonsApplySpell {

static std::vector<RE::BGSKeyword*> followers_keyword{};
static std::vector<RE::BGSKeyword*> summons_keyword{};

auto is_valid_spell_to_apply(const RE::SpellItem* spell_to_apply) -> bool
{
  if (!spell_to_apply) {
    return false;
  }

  if (spell_to_apply->GetDelivery() == RE::MagicSystem::Delivery::kSelf &&
      spell_to_apply->GetSpellType() == RE::MagicSystem::SpellType::kAbility) {
    return true;
  }

  return false;
}

auto apply_spell_handle(RE::Actor* from,
                        RE::Actor* to,
                        RE::BGSListForm* keywords,
                        RE::BGSListForm* spells,
                        std::vector<RE::BGSKeyword*>* actors_keywords) -> void
{
  if (!from || !to || !keywords || !spells) {
    return;
  }

  const auto length_kw = keywords->forms.size();
  const auto length_sp = spells->forms.size();

  for (std::uint32_t index = 0u; index < length_kw && index < length_sp; index++) {
    let keyword_form = keywords->forms[index];
    let spell_form = spells->forms[index];
    if (!keyword_form || !spell_form) {
      continue;
    }

    let keyword = keyword_form->As<RE::BGSKeyword>();
    let spell = spell_form->As<RE::SpellItem>();

    if (!spell || !keyword) {
      continue;
    }

    if (is_valid_spell_to_apply(spell)) {

      if (actors_keywords) {
        if (Core::vector_keyword_form_exist(actors_keywords, keyword)) {
          to->AddSpell(spell);
        } else {
          to->RemoveSpell(spell);
        }
        continue;
      }

      if (Core::try_has_absolute_keyword(from, keyword)) {
        to->AddSpell(spell);
      } else {
        to->RemoveSpell(spell);
      }
    }
  }
}

auto update(RE::Actor* actor) -> void
{
  if (!actor) {
    return;
  }

  followers_keyword.clear();
  summons_keyword.clear();

  letr config = Config::get_singleton();

  if (actor->IsPlayerRef()) {
    let player_followers = Core::get_player_followers();
    for (auto player_follower : player_followers) {

      apply_spell_handle(actor,
                         player_follower,
                         config.followers_summons_apply_spell().keywords_from_player_to_followers(),
                         config.followers_summons_apply_spell().spells_from_player_to_followers(),
                         nullptr);

      let follower_actor = player_follower->GetActorBase();
      if (!follower_actor) {
        continue;
      }

      follower_actor->ForEachKeyword([&](RE::BGSKeyword* keyword) {
        followers_keyword.push_back(keyword);
        return RE::BSContainer::ForEachResult::kContinue;
      });
    }
    apply_spell_handle(actor,
                       actor,
                       config.followers_summons_apply_spell().keywords_to_player_from_followers(),
                       config.followers_summons_apply_spell().spells_to_player_from_followers(),
                       &followers_keyword);
  }

  let summons = Core::get_commanded_actors(actor);
  if (summons) {
    for (const auto& summon : *summons) {
      if (summon.commandedActor && summon.commandedActor.get() && summon.commandedActor.get().get()) {

        apply_spell_handle(actor,
                           summon.commandedActor.get().get(),
                           config.followers_summons_apply_spell().keywords_from_player_to_summons(),
                           config.followers_summons_apply_spell().spells_from_player_to_summons(),
                           nullptr);

        let summon_actor = summon.commandedActor.get()->GetActorBase();
        if (!summon_actor) {
          continue;
        }

        summon_actor->ForEachKeyword([&](RE::BGSKeyword* keyword) {
          followers_keyword.push_back(keyword);
          return RE::BSContainer::ForEachResult::kContinue;
        });
      }
    }
    apply_spell_handle(actor,
                       actor,
                       config.followers_summons_apply_spell().keywords_to_player_from_summons(),
                       config.followers_summons_apply_spell().spells_to_player_from_summons(),
                       &summons_keyword);
  }
}

}
