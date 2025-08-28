package com.Starygin.service.mapper;

import com.Starygin.model.entity.Owner;
import com.Starygin.model.entity.Pet;
import com.Starygin.service.dto.OwnerSimpleDto;
import com.Starygin.service.dto.PetDto;
import com.Starygin.service.dto.PetSimpleDto;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.List;

@Component
public class PetMapper implements Mapper<Pet, PetDto> {
    @Override
    public Pet toEntity(PetDto dto) {
        Pet pet = new Pet();
        pet.setId(dto.id());
        pet.setName(dto.name());
        pet.setBirthday(dto.birthday());
        pet.setColor(dto.color());
        pet.setTailLength(dto.tailLength());

        Owner owner = new Owner(dto.owner().name(), dto.owner().birthday());
        owner.setId(dto.owner().id());

        pet.setOwner(owner);

        return pet;
    }

    @Override
    public PetDto toDto(Pet entity) {
        OwnerSimpleDto owner = new OwnerSimpleDto(entity.getOwner().getId(),
                entity.getOwner().getName(), entity.getOwner().getBirthday());
        List<PetSimpleDto> friends = new ArrayList<>();

        for (Pet pet : entity.getFriends()) {
            friends.add(new PetSimpleDto(pet.getId(), pet.getName(), pet.getBirthday(), pet.getColor(), pet.getTailLength()));
        }

        return new PetDto(entity.getId(), entity.getName(), entity.getBirthday(), entity.getColor(), entity.getTailLength(), owner, friends);
    }
}
